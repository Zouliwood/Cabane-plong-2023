#include <opencv2/opencv.hpp>
#include <thread>
#include <unistd.h>
#include <functional>


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
 * @param result     Objet de retour derun la fonction
 * @param seuil      Le seuil permettant de délimiter la marge d'erreur
 */
void masque(const Mat &image, const Mat &background, Mat &result, int seuil = 5) {

    auto exp = [image, background, seuil, result](int i, int j, int rayon) mutable {
        if (abs(background.at<uchar>(i, j) - image.at<uchar>(i, j)) < seuil)
            result.at<uchar>(i, j) = 0;
    };

    

    run<function<void(int, int, int)>>(image, exp);
}




/*
affichage des histogrammes (rgb)
*/

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



int main(int argc, char** argv ){


    //TODO: test avec d'autres espaces colorimétriques
    Mat imagecolor = imread( "../images/bird.jpg", IMREAD_COLOR);


    histogramme(imagecolor);
    
    Mat imagea = imread("../img/a.jpg", IMREAD_GRAYSCALE);
    Mat images = imread("../img/s.jpg", IMREAD_GRAYSCALE);

    Mat image_masque = imagea.clone();
    masque(imagea, images, image_masque, 50);

    Mat img_result = image_masque.clone();
    morphGradient(image_masque, img_result, 10);

    cout << imwrite("../save.jpg", img_result) << endl;

    vector<Vec3b> couleurs = listcolours(imagecolor);
    
    /*for(Vec3b v:couleurs){
        cout << v << endl;
    }*/
   
    imshow("Gradiant Morphologique", img_result);
    waitKey(0);
    destroyAllWindows();

    return 0;

}
