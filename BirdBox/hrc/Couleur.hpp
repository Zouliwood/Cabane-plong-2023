#ifndef BIRDBOX_COULEUR
#define BIRDBOX_COULEUR

#include <opencv2/opencv.hpp>

using namespace cv;

class Couleur {

public:

    int histogramme(const Mat &src);
    vector<Vec3b> listcolours(Mat imc);
};

#endif 