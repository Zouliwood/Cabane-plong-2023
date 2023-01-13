#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//dilatation - érosion => contour des objets
//gradient: interne, externe, morphologique

void erosion_dilatation(const Mat& image, Mat& img_clone, int rayon=2, bool flag=false){
    for (int i = rayon; i < img_clone.rows-rayon; i++){
        for (int j = rayon; j < img_clone.cols-rayon; j++){

            Mat frag = image(Range(i-rayon, i+rayon), Range(j-rayon, j+rayon));//utiliser d'autres formes qu'un carré
            double minVal, maxVal;
            Point minLoc, maxLoc;
            minMaxLoc( frag, &minVal, &maxVal, &minLoc, &maxLoc );

            img_clone.at<uchar>(i, j) = (flag)?(int)maxVal:(int)minVal;
        }
    }
}

//affichage des histogrammes (rgb)

int histogramme(Mat src){

    if( src.empty() ){
        return EXIT_FAILURE;
    }

    vector<Mat> bgr_planes;
    split( src, bgr_planes );       //separation en rgb

//paramètres pour les histogrammes
    int histSize = 256;
    float range[] = { 0, 256 }; 
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;

//calcul des histogrammes
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

//représentation des histogrammes 
    for( int i = 1; i < histSize; i++ ){
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }

    imshow("Source", src );
    imshow("Histogrammes", histImage );
    waitKey();
    return EXIT_SUCCESS;

}

int main(int argc, char** argv ){


    //TODO: test avec d'autres espaces colorimétriques
    Mat image = imread( "../bird.jpg", IMREAD_GRAYSCALE);
    Mat imagecolor = imread( "../bird.jpg", IMREAD_COLOR);

    histogramme(imagecolor);

    //TODO: essayer de supprimer les elements != oiseaux

    Mat img_clone1 = image.clone();
    Mat img_clone2 = image.clone();

    erosion_dilatation(image, img_clone1, 2, true);
    erosion_dilatation(image, img_clone2, 2, false);

    imshow("Gradiant Morphologique", img_clone1-img_clone2);

    waitKey(0);
    return 0;

}
