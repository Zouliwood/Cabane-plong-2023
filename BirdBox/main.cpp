#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"

using namespace cv; 

int main(int argc, char **argv) {
   
    Mat el2 = imread("../images/cabane oiseau 1.jpg", IMREAD_GRAYSCALE);


    Mat el3 = imread("../images/moineau.jpg", IMREAD_COLOR);
    Mat el4 = imread("../images/corbeau.jpg", IMREAD_COLOR);

    //test histogrammes


    //TODO: fix
    double c1= Couleur::HistComp(el3,el4);
    double c2= Couleur::HistComp(el3,el3);

    //test caméra

    Mat camera=Camera::getPic();

    //test taille 

    float sizeBird = Calcul::sizeOiseau(el2);

    cout << sizeBird <<" cm" << endl;

    //test couleur dominante

   // Mat bird = el3.clone();

    Vec3b v = Couleur::MostFrequentColor(el3);

    cout << "couleur dominante : " << v << endl;
  
    destroyAllWindows();

    return 0;

}
 