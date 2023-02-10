#include "../hrc/Image.hpp"

/**
 * Permet de lancer une tache en la répartissant sur x thread
 * @param  image  Image de référence
 * @param  lambda Fonction lambda contenant les calculs à réaliser
 * @param  rayon  Dimensions facultatives du calque
 */
void Image::run(const Mat &image, function<void(int, int, int)> &&lambda, int rayon) {

    static const int nbThread = int(sysconf(_SC_NPROCESSORS_ONLN));
    static vector<thread> listThread;
    int range = (image.rows - rayon) / nbThread;

    for (int iThread = 0; iThread < nbThread; ++iThread) {
        listThread.emplace_back([&, iThread, rayon, range, lambda]() {

            for (int i = (iThread == 0) ? rayon : range * iThread; i < range * (iThread + 1); i++) {
                for (int j = rayon; j < image.cols - rayon; j++) {
                    lambda(i, j, rayon);
                }
            }

        });
    }

    for (int i = 0; i < nbThread; ++i)
        listThread.at(i).join();

    listThread.clear();

}

/**
 * Permet de calculer le gradient morphologique d'une image
 * @param image  Image à partir de laquelle on veut obtenir le gradient morphologique
 * @param result Objet sur lequel la fonction doit travailler
 * @param rayon  Dimensions du calque permettant de calculer l'érosion et la dilatation
 */
void Image::morphGradient(const Mat &image, Mat &result, int rayon) {
    Mat img_erosion = Mat::zeros(image.size(), image.type()), img_dilatation = Mat::zeros(image.size(), image.type());

    auto exp = [&](int i, int j, int rayon) mutable {
        Mat frag = image(Range(i - rayon, i + rayon), Range(j - rayon, j + rayon));
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(frag, &minVal, &maxVal, &minLoc, &maxLoc);

        img_erosion.at<uchar>(i, j) = (int) minVal;
        img_dilatation.at<uchar>(i, j) = (int) maxVal;
    };

    run(image, exp, rayon);

    result = img_dilatation - img_erosion;
}

/**
 * Permet d'isoler un élément en fonction d'une image de référence
 * @param image      Image contenant l'objet à isoler
 * @param background Image de référence
 * @param result     Objet sur lequel la fonction doit travailler
 * @param seuil      Le seuil permettant de délimiter la marge d'erreur
 */
void Image::masque(const Mat &image, const Mat &background, Mat &result, int seuil) {

    auto exp = [&](int i, int j, int rayon) mutable {
        if (abs(background.at<uchar>(i, j) - image.at<uchar>(i, j)) < seuil)
            result.at<uchar>(i, j) = 0;
    };

    run(image, exp);
}

/**
 * Permet de placer un point dans une image
 * @param image Image où l'on veut placer le point
 * @param point Position du point
 * @param rayon Dimension du point à placer
 */
void Image::drawCross(Mat &image, Point &point, int rayon) {
    Point pointXLeft(point.x - rayon, point.y);
    Point pointXRight(point.x + rayon, point.y);
    line(image, pointXLeft, pointXRight, 0, 2);

    Point pointYLeft(point.x, point.y - rayon);
    Point pointYRight(point.x, point.y + rayon);
    line(image, pointYLeft, pointYRight, 0, 2);
}

/**
 * Permet d'obtenir les bordures d'objets présents su une image
 * @param  masque Image contenant les objets
 * @return        Liste de bordures d'objets
 */
vector<vector<Point>> Image::getBordures(const Mat &masque) {

    vector<vector<Point>> listBordures;
    findContours(masque, listBordures, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    sort(listBordures.begin(), listBordures.end(),
         [](const vector<Point> &listPoint1, const vector<Point> &listPoint2) {
             return contourArea(listPoint1, false) < contourArea(listPoint2, false);
         });

    return listBordures;

}

/**
 * Permet de localiser le plus grand objet dans une image
 * @param  image  Image où l'on veut placer le point
 * @param  masque Image où l'on veut détecter l'objet
 * @param  rayon  Dimension du point que l'on veut placer
 * @return        Coordonnées du point que l'on à placé
 */
Point Image::getObject(Mat &image, const Mat &masque) {

    vector<vector<Point>> listBordures = getBordures(masque);
    Point point(0, 0);

    if (!listBordures.empty()) {
        Moments m = moments(listBordures[listBordures.size() - 1]);
        point = Point(int(m.m10 / m.m00), int(m.m01 / m.m00));
        drawCross(image, point);
    }

    return point;
}

/**
 * Permet de récupérer la taille du plus grand objet présent sur le calque (en pixel)
 * @param  masque Le calque contenant l'objet à mesurer
 * @return        La taille en px de l'objet
 */
float Image::getPxSizeObject(const Mat &masque) {

    vector<vector<Point>> listBordures = getBordures(masque);
    float pxObjSize = 0;

    if (!listBordures.empty()) {
        vector<Point> largestContour = listBordures[listBordures.size() - 1];
        Point2f center;
        minEnclosingCircle(largestContour, center, pxObjSize);
    }

    return pxObjSize * 2;
}
