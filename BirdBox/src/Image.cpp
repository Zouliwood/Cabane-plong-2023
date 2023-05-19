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
    Mat image_erosion = Mat::zeros(image.size(), image.type()), image_dilatation = Mat::zeros(image.size(), image.type());

    auto exp = [&](int i, int j, int rayon) mutable {
        Mat frag = image(Range(i - rayon, i + rayon), Range(j - rayon, j + rayon));
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(frag, &minVal, &maxVal, &minLoc, &maxLoc);

        image_erosion.at<uchar>(i, j) = (int) minVal;
        image_dilatation.at<uchar>(i, j) = (int) maxVal;
    };

    run(image, exp, rayon);

    result = image_dilatation - image_erosion;
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
 * Même principe que masque, mais pour des images en couleur
 * @param image      Image contenant l'objet à isoler
 * @param background Image de référence
 * @param result     Objet sur lequel la fonction doit travailler
 * @param seuil      Le seuil permettant de délimiter la marge d'erreur
 */
void Image::masquecolor(const Mat &image, const Mat &background, Mat &result, int seuil) {
    for (int i=0;i<image.rows;i++){
        for (int j=0;j<image.cols;j++){
           // cout<<"valeur " << norm(background.at<Vec3b>(i, j) - image.at<Vec3b>(i, j))<< endl;
            if (norm(background.at<Vec3b>(i, j) - image.at<Vec3b>(i, j)) < seuil)
                result.at<Vec3b>(i, j) = 0;
        }
    }
}






/**
 * Rogne une image afin de conserver la différence avec une référence
 * @param image      Image contenant l'objet à isoler
 * @param background Image de référence
 * @param result     Objet sur lequel la fonction doit travailler
 */
void Image::cropImage(Mat &image){

    Rect bbox;
    Mat grayImg;
    cvtColor(image, grayImg, COLOR_BGR2GRAY);
    threshold(grayImg, grayImg, 127, 255, THRESH_BINARY);
    bbox = boundingRect(grayImg);


    // Trouver les coordonnées des pixels non nuls
    // vector<Point> nonZeroPoints;
    // findNonZero(grayImg, nonZeroPoints);

    // Calculer le rectangle de la zone à rogner
    // Rect boundingRect = cv::boundingRect(nonZeroPoints);

    Mat croppedImg = image(bbox);

    image = croppedImg;

}

Mat Image::cropImg(Mat &image, Mat &oiseau){

    vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

        int largestContourIndex = 0;
        double largestContourArea = 0.0;
        for (int i = 0; i < contours.size(); ++i) {
            double currentContourArea = contourArea(contours[i]);
            if (currentContourArea > largestContourArea) {
                largestContourArea = currentContourArea;
                largestContourIndex = i;
            }
        }

        Mat image_cpy = oiseau.clone();
        drawContours(image_cpy, contours, -1, Scalar(0, 255, 0), 2);
    
        imwrite("../images/oiseau_avec_contours.jpg", image_cpy);


        return image_cpy;
}


void Image::bnry(Mat thresh){
    Mat img_color = imread("../images/idkatthispoint.jpg", IMREAD_COLOR);
    Mat img_gray;
    cvtColor(img_color, img_gray, COLOR_BGR2GRAY);
    Mat blurred;
    GaussianBlur(img_gray, blurred, Size(5, 5), 0);
    threshold(blurred, thresh, 150, 255, THRESH_BINARY);
    imwrite("../images/binary.jpg", thresh);
 }


Mat Image::msq(Mat oiseau, Mat ref){
    Mat diffImage;
    absdiff(ref, oiseau, diffImage);

    Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);

    float thold = 30.0f;
    float dist;

    for(int j=0; j<diffImage.rows; ++j){
        for(int i=0; i<diffImage.cols; ++i)
        {
            Vec3b pix = diffImage.at<Vec3b>(j,i);

            dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
            dist = sqrt(dist);

            if(dist>thold)
            {
                foregroundMask.at<unsigned char>(j,i) = 255;
            }
        }
    }

    imwrite("../images/idkatthispoint.jpg", foregroundMask);

     Mat blurred;
    GaussianBlur(foregroundMask, blurred, Size(5, 5), 0);
    Mat thresh;
    threshold(blurred, thresh, 150, 255, THRESH_BINARY);
    imwrite("../images/binary.jpg", thresh);

    return thresh;
}

vector<Point> Image::addContours(Mat thrwdwesh, Mat imagdwede_cpy){
    Mat thresh;
    Mat oiseau = imread("../images/B.jpg", IMREAD_COLOR);
    Mat image_cpy = oiseau.clone();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

        int largestContourIndex = 0;
        double largestContourArea = 0.0;
        for (int i = 0; i < contours.size(); ++i) {
            double currentContourArea = contourArea(contours[i]);
            if (currentContourArea > largestContourArea) {
                largestContourArea = currentContourArea;
                largestContourIndex = i;
            }
        }


    drawContours(image_cpy, contours, -1, Scalar(0, 255, 0), 2);
    //imwrite("../images/contours_none_image1.jpg", image_cpy);
    //return contours[largestContourIndex];

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
 * Permet d'obtenir les bordures d'objets présents sur une image
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

    // for(vector<Point> p1: listBordures){
    //     cout << endl;
    //     for(Point p2: p1){
    //         cout << p2 << endl;
    //     }
    // }

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
    cout << "pxObjSize: "<< pxObjSize *2<< endl;
    return pxObjSize * 2;
}
