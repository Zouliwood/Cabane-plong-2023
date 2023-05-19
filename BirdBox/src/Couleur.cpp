#include "../hrc/Couleur.hpp"
#include <functional>
#include <thread>
#include <unistd.h>


using namespace std;
using namespace cv;

/**
 * Création d'un histogramme à partir d'une image
 * @param   src   Image dont on souhaite avoir l'histogramme
 * @param   img   Boolean indiquant le souhait de générer une image avec l'histogramme (graphe)
 * @return              Histogramme
 */
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

/**
 * Création d'un graphe à partir d'histogrammes
 * @param   histImage   Matrice de l'image pour le graphe
 * @param   b_hist      Histogramme pour les valeur B des RGB
 * @param   g_hist      Histogramme pour les valeur G des RGB
 * @param   r_hist      Histogramme pour les valeur R des RGB
 * @return              Image contenant le graphe
 */
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

/**
 * Permet de comparer deux histogrammes
 * @param   img_src    Image source prise dans la cabane
 * @param   img_comp   Image de référence pour comparer
 * @return            Taux de ressemblance (en pourcentage)
 */
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

/**
 * Permet de récupérer toutes les couleurs d'une image
 * @return            vecteur des valeurs RGB de chaque couleur
 */
vector<Vec3b> listcolours(Mat imc){
    vector<Vec3b> colors;

    for(int y=0;y<imc.rows;y++){
        for(int x=0;x<imc.cols;x++){

            Vec3b c =imc.at<Vec3b>(Point(x,y));
            
            colors.push_back(c);
        }

    }

    return colors;

}

struct Vec3bComparator{
    bool operator()(const Vec3b& a, const Vec3b& b) const
    {
        return tie(a[0], a[1], a[2]) < tie(b[0], b[1], b[2]);
    }
};


//TO DO: Tester que la couleur ne fait pas partie de celles dans la ref
Vec3b Couleur::MostFrequentColor(Mat image){
    map<Vec3b, int,Vec3bComparator> color_freq;

    for (int i=0;i<image.cols;i++){
        for (int j=0;j<image.rows;j++){
            Vec3b pixel = image.at<cv::Vec3b>(j, i);
            color_freq[pixel]++;
        }
    }

    // Find the color with the highest frequency count
    Vec3b most_frequent_color;
    int max_freq = 0;

    for (auto const& element : color_freq) {

        if (element.second> max_freq && element.first != Vec3b{0,255,0}){
            most_frequent_color = element.first;
            max_freq = element.second;
        }
    }

    return most_frequent_color;

}
