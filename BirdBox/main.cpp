#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"
#include "hrc/Bird.hpp"
#include "hrc/KNearestNeighbors.hpp"

//ip raspberry : 192.168.1.10


using namespace cv; 


int main(int argc, char **argv) {
   
    Mat el2 = imread("../images/cabane oiseau 1.jpg", IMREAD_GRAYSCALE);
    Mat ref = imread("../images/cabane vide.jpg", IMREAD_COLOR);
    Mat oiseau = imread("../images/cabane oiseau 1.jpg", IMREAD_COLOR);

    Mat el3 = imread("../images/moineau.jpg", IMREAD_COLOR);
    Mat el4 = imread("../images/corbeau.jpg", IMREAD_COLOR);

    //test histogrammes


    //TODO: fix
    // double c1= Couleur::HistComp(el3,el4);
    // double c2= Couleur::HistComp(el3,el3);

    //test caméra

    Mat camera=Camera::getPic("../images/src_cabane.jpg");

    //test taille 

    float sizeBird = Calcul::sizeOiseau(el2);

    cout << sizeBird <<" cm" << endl;

    //test couleur dominante

    Mat bird=oiseau.clone();

    Image::masquecolor(oiseau,ref,bird,5);

    Image::cropImage(bird);
    imwrite("../images/src_oiseau.jpg", bird);

    

     Vec3b domColor = Couleur::MostFrequentColor(bird);

     cout << "couleur dominante : " << domColor << endl;
  

    //algo knn

    Bird b(domColor,sizeBird,"unknown");

    vector<Bird> listb{Bird(Vec3b(255, 55, 0), 45, "Cockatiel"),
            Bird(Vec3b(255, 55, 0), 50, "Triton cockatoo"),
            Bird(Vec3b(255, 0, 0), 15, "Gouldian finch"),
            Bird(Vec3b(255, 255, 255), 32, "Columba"),
            Bird(Vec3b(255, 245, 250), 35, "Columba")};
        
    
    KNearestNeighbors knn(listb);
    
    vector<Bird> voisins = knn.getKNN(b, 1);

    cout << "espèce " << voisins.at(0).getName() << endl;

    return 0;

}
 