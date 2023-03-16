#include "../hrc/Calcul.hpp"

//TO DO: modifier selon les dimensions réelles de la cabane
//grillage mur:
//50 px -> 5cm
int pxWall=50;
int distWall=5;
//grillage sol:
int pxFloor=45;
int distFloor=4;



/**
 * Permet de calculer la distance d'un objet via deux images
 * @param  imageLeft  Image gauche contenant l'objet
 * @param  imageRight Image droite contenant l'objet
 * @return            La distance séparant l'objet de la caméra
 */
float Calcul::distanceObject(Mat &imageLeft, Mat &imageRight) {

    Mat masqueLeft = Mat::zeros(imageLeft.size(), imageLeft.type());
    Image::morphGradient(imageLeft, masqueLeft);

    Mat masqueRight = Mat::zeros(imageRight.size(), imageRight.type());
    Image::morphGradient(imageRight, masqueRight);

    Point positionLeft = Image::getObject(imageLeft, masqueLeft);
    Point positionRight = Image::getObject(imageRight, masqueRight);

    return (!(positionLeft.x && positionRight.x)) ? -1 :
           Camera::getUniqueInstance().getDepth(positionLeft, positionRight, imageRight);
}

/**
 * Permet de calculer la taille réelle d'un objet d'une image
 * @param  image  Image gauche contenant uniquement l'objet
 * @return            La taille de l'objet
 */
float Calcul::sizeOiseau(Mat &image) {
    //TO DO: Faire le calcul en considérant la profondeur(oiseau dans différents plans)

    Mat ref = imread("../images/cabane vide.jpg", IMREAD_GRAYSCALE);

    Mat oiseau=image.clone();

    Image::masque(image,ref,oiseau);

    float pxSize= Image::getPxSizeObject(oiseau);

    cout<<"taille sur image: "<< pxSize << endl;

    float taille = pxSize*distWall/pxWall;
   
    return taille;

}



/**
 * Permet de calculer la taille d'un objet (centré) via deux images
 * @param  imageLeft  Image gauche contenant l'objet
 * @param  imageRight Image droite contenant l'objet
 * @return            La distance séparant l'objet de la caméra
 */
float Calcul::sizeObject(Mat &imageLeft, Mat &imageRight) {

    Mat masqueLeft = Mat::zeros(imageLeft.size(), imageLeft.type());
    Image::morphGradient(imageLeft, masqueLeft);

    float sizeObj = Image::getPxSizeObject(masqueLeft);
    if (sizeObj == 0) return -1;

    float ratio = sizeObj / float(masqueLeft.size().height);
    ratio *= tan(Camera::getUniqueInstance().getAlpha() / 2);

    float distance = distanceObject(imageLeft, imageRight);

    return (distance < 0) ? -1 : 2 * ratio * distance;
}

//TODO: Implémenter la fonction permettant de calculer la surface d'un oiseau.
float Calcul::angleSolid(){
    return 0;
}
