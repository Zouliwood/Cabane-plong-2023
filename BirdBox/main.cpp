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

int main(int argc, char** argv ){

    //TODO: test avec d'autres espaces colorimétriques
    Mat image = imread( "../bird.jpg", IMREAD_GRAYSCALE);

    //TODO: essayer de supprimer les elements != oiseaux

    Mat img_clone1 = image.clone();
    Mat img_clone2 = image.clone();

    erosion_dilatation(image, img_clone1, 2, true);
    erosion_dilatation(image, img_clone2, 2, false);

    imshow("Gradiant Morphologique", img_clone1-img_clone2);

    waitKey(0);
    return 0;

}
