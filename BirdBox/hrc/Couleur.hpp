#ifndef BIRDBOX_COULEUR
#define BIRDBOX_COULEUR

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Couleur {

    public:

        static Mat histogramme(const Mat &src,bool img);
        char * idBird(Mat img_src);
        static double HistComp(Mat src, Mat comp);
        static Mat grapheHist(Mat histimg,Mat b_hist, Mat g_hist, Mat r_hist);
        vector<Vec3b> listcolours(Mat imc);
        static Vec3b MostFrequentColor(Mat image);
};

#endif //BIRDBOX_COULEUR