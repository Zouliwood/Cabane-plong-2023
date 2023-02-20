#include "../hrc/Couleur.hpp"
#include <functional>
#include <thread>

using namespace std;

Mat Couleur::histogramme(const Mat &src, bool img) {

    /*if (src.empty()) {
        return;
    }*/

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

    if(img){
        histImage= grapheHist(histImage,b_hist, g_hist,r_hist);
    }
    
    //imshow("Source", src);
    imshow("Histogrammes", histImage);
    waitKey();
    return histImage;

}

Mat Couleur::grapheHist(Mat histImage,Mat b_hist, Mat g_hist, Mat r_hist){
//représentation des histogrammes 
    int bin_w = cvRound((double) 512 / 256);

    for (int i = 1; i < 256; i++) {
        line(histImage, Point(bin_w * (i - 1), 400 - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), 400 - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), 400 - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), 400 - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), 400 - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), 400 - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    return histImage;
}

char * Couleur::idBird(Mat img_src){
    
}


double Couleur::HistComp(Mat img_src, Mat img_comp){


    
    Mat hsv_src, hsv_comp;
    cvtColor( img_src, hsv_src, COLOR_BGR2HSV );
    cvtColor( img_comp, hsv_comp, COLOR_BGR2HSV );

    int h_bins = 50, s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };
    int channels[] = { 0, 1 };
    Mat hist_src, hist_comp;

    calcHist( &hsv_src, 1, channels, Mat(), hist_src, 2, histSize, ranges, true, false );
    normalize( hist_src, hist_src, 0, 1, NORM_MINMAX, -1, Mat() );
    calcHist( &hsv_comp, 1, channels, Mat(), hist_comp, 2, histSize, ranges, true, false );
    normalize( hist_comp, hist_comp, 0, 1, NORM_MINMAX, -1, Mat() );


    double comp=compareHist( hist_src, hist_comp, 0 )*100;


    cout<<"compa: "<< comp<<endl;

    return comp;
}