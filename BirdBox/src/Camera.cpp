#include "../hrc/Camera.hpp"

const Camera Camera::instance = Camera();

Camera::Camera(float lnBase, float alpha)
        : lnBase(lnBase), alpha(alpha) {}

const Camera &Camera::getUniqueInstance() {
    return instance;
}

/**
 * Permet de calculer la distance qui sépare la caméra d'un objet
 * @param  pointLeft  Position de l'objet sur l'image gauche
 * @param  pointRight Position de l'objet sur l'image droite
 * @param  imageRight Image droite
 * @return            Distance entre la caméra et l'objet en centimètre
 */
float Camera::getDepth(const Point &pointLeft, const Point &pointRight, const Mat &imageRight) const {

    int pxFocale = int((imageRight.cols * 0.5) / tan(alpha * 0.5 * CV_PI / 180.0));

    int disparity = pointLeft.x - pointRight.x;

    return abs((lnBase * float(pxFocale)) / float(disparity));

}

float Camera::getAlpha() const {
    return alpha;
}

float Camera::getLnBase() const {
    return lnBase;
}