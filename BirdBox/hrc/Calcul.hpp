#ifndef BIRDBOX_CALCUL_HPP
#define BIRDBOX_CALCUL_HPP

#include "Camera.hpp"
#include "Image.hpp"

class Calcul {

public:


    static float distanceObject(Mat &imageLeft, Mat &imageRight);

    static float sizeObject(Mat &imageLeft, Mat &imageRight);

    static float angleSolid();

    static float sizeOiseau(Mat &image);
};

#endif //BIRDBOX_CALCUL_HPP
