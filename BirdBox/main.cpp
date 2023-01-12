#include <opencv2/opencv.hpp>
#include <thread>

using namespace cv;
using namespace std;

static const auto nbThread = (int) sysconf(_SC_NPROCESSORS_ONLN);
static vector<thread> listThread;

/**
 * Permet de lancer une tache en la répartissant sur x thread
 * @tparam F      Signature de l'expression lambda
 * @param  image  Image de référence
 * @param  lambda Fonction lambda contenant les calculs à réaliser
 * @param  rayon  Dimensions facultatives du calque
 */
template<typename F>
void run(const Mat &image, F &&lambda, int rayon = 0) {
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
 * @param result Objet de retour de la fonction
 * @param rayon  Dimensions du calque permettant de calculer l'érosion et la dilatation
 */
void morphGradient(const Mat &image, Mat &result, int rayon = 5) {
    Mat img_erosion = image.clone(), img_dilatation = image.clone();

    auto exp = [image, img_erosion, img_dilatation](int i, int j, int rayon) mutable {
        Mat frag = image(Range(i - rayon, i + rayon), Range(j - rayon, j + rayon));
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(frag, &minVal, &maxVal, &minLoc, &maxLoc);

        img_erosion.at<uchar>(i, j) = (int) minVal;
        img_dilatation.at<uchar>(i, j) = (int) maxVal;
    };

    run<function<void(int, int, int)>>(image, exp, rayon);

    result = img_dilatation - img_erosion;
}

/**
 * Permet d'isoler un élément en fonction d'une image de référence
 * @param image      Image contenant l'objet à isoler
 * @param background Image de référence
 * @param result     Objet de retour de la fonction
 * @param seuil      Le seuil permettant de délimiter la marge d'erreur
 */
void masque(const Mat &image, const Mat &background, Mat &result, int seuil = 5) {

    auto exp = [image, background, seuil, result](int i, int j, int rayon) mutable {
        if (abs(background.at<uchar>(i, j) - image.at<uchar>(i, j)) < seuil)
            result.at<uchar>(i, j) = 0;
    };

    run<function<void(int, int, int)>>(image, exp);
}

void histogramme() {
    //TODO()
}

int main(int argc, char **argv) {

    //TODO: test avec d'autres espaces colorimétriques
    Mat imagea = imread("../img/a.jpg", IMREAD_GRAYSCALE);
    Mat images = imread("../img/s.jpg", IMREAD_GRAYSCALE);

    Mat image_masque = imagea.clone();
    masque(imagea, images, image_masque, 50);

    Mat img_result = image_masque.clone();
    morphGradient(image_masque, img_result, 10);

    cout << imwrite("../save.jpg", img_result) << endl;

    //imshow("Gradiant Morphologique", img_result);
    //waitKey(0);
    destroyAllWindows();

    return 0;

}
