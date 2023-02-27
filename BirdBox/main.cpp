#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"




int main(int argc, char **argv) {
   
    Mat el1 = imread("../images/bird.jpg", IMREAD_GRAYSCALE);
    Mat el2 = imread("../images/cabane oiseau 1", IMREAD_GRAYSCALE);
    float szObject = Calcul::sizeObject(el1, el2);
    cout << "sizeObject: " << szObject << endl;

    Mat el3 = imread("../images/moineau.jpg", IMREAD_COLOR);
    Mat el4 = imread("../images/corbeau.jpg", IMREAD_COLOR);

    //imshow("Histogrammes", el3);

    double c1= Couleur::HistComp(el3,el4);
    double c2= Couleur::HistComp(el3,el3);

    Mat camera=Camera::getPic(2);

    float pxSize= Image::getPxSizeObject(el2);

    cout<<"taille: "<< pxSize << endl;

    destroyAllWindows();

    return 0;

}
 