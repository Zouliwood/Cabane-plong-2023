#include "hrc/Couleur.hpp"
#include <functional>
#include <thread>

using namespace std;

/*
affichage des histogrammes (rgb)
*/


int Couleur::histogramme(const Mat &src) {

    if (src.empty()) {
        return EXIT_FAILURE;
    }

    vector<Mat> bgr_planes;
    split(src, bgr_planes);       //separation en rgb

//paramètres pour les histogrammes
    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange[] = {range};
    bool uniform = true, accumulate = false;

//calcul des histogrammes
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, nullptr, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, nullptr, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, nullptr, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

//représentation des histogrammes 
    for (int i = 1; i < histSize; i++) {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    imshow("Source", src);
    imshow("Histogrammes", histImage);
    waitKey();
    return EXIT_SUCCESS;

}


vector<Vec3b> Couleur::listcolours(Mat imc) {
    vector<Vec3b> colors;

    for (int y = 0; y < imc.rows; y++) {
        for (int x = 0; x < imc.cols; x++) {

            Vec3b c = imc.at<Vec3b>(Point(x, y));

            colors.push_back(c);
        }

    }

    return colors;

}