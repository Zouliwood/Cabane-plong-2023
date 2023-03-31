#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"

using namespace cv; 

int main(int argc, char **argv) {
   
    Mat el2 = imread("../images/cabane oiseau 1.jpg", IMREAD_GRAYSCALE);


    Mat el3 = imread("../images/moineau.jpg", COLOR_BGR2HSV);
    Mat el4 = imread("../images/corbeau.jpg", COLOR_BGR2HSV);

    //test histogrammes


    //TODO: fix
    //double c1= Couleur::HistComp(el3,el4);
    //double c2= Couleur::HistComp(el3,el3);

    //test caméra

    Mat camera=Camera::getPic();

    //test taille 

    float sizeBird = Calcul::sizeOiseau(el2);

    cout << sizeBird <<" cm" << endl;
  
  
    destroyAllWindows();

    return 0;

}
 