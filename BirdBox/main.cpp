#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"
#include "hrc/Bird.hpp"
#include "hrc/KNearestNeighbors.hpp"

//ip raspberry : 192.168.1.10


using namespace cv; 


int main(int argc, char **argv) {

   while(1){
        cout << "Photo Cabane vide";
        cin.ignore();
        Mat ref=Camera::getPic("../images/src_cabane.jpg");
        cout << "Photo Oiseau";
        cin.ignore();
        Mat oiseau=Camera::getPic("../images/oiseau.jpg");

        
        // Mat ref = imread("../images/A.jpg", IMREAD_COLOR);
        // Mat oiseau = imread("../images/B.jpg", IMREAD_COLOR);

        Mat t = Image::msq(oiseau,  ref);

        Mat t_color = Image::isolate(t,oiseau);

        t = imread("oiseau_isole.jpg", IMREAD_GRAYSCALE);


        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(t, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

        int largestContourIndex = 0;
        double largestContourArea = 0.0;
        for (int i = 0; i < contours.size(); ++i) {
            double currentContourArea = contourArea(contours[i]);
            if (currentContourArea > largestContourArea) {
                largestContourArea = currentContourArea;
                largestContourIndex = i;
            }
        }

        Mat image_cpy = oiseau.clone();
        drawContours(image_cpy, contours, -1, Scalar(0, 255, 0), 2);
    
        imwrite("../images/image_avec_contours.jpg", image_cpy);


        float sizeBird = Calcul::sizeOiseau(image_cpy);
        cout << sizeBird <<" cm" << endl;

        if(sizeBird>0){
            Rect bdRect = boundingRect(contours[largestContourIndex]);

                

            Mat croppedImage = t_color(bdRect);

            imwrite("../images/cropped_image.jpg", croppedImage);
            
            Vec3b domColor = Couleur::MostFrequentColor(croppedImage);

            cout << "couleur dominante : " << domColor << endl;

            //algo knn

            
        Bird b(&domColor,sizeBird);

       Vec3b colorsMountainBluebird[NB_COLOR] = {Vec3b(86, 178, 233)};
       Vec3b colorsRobin[NB_COLOR] = {Vec3b(201, 162, 57)};
       Vec3b colorsAtlanticCanary[NB_COLOR] = {Vec3b(225, 225, 51)};

        vector<Bird> listb{
                Bird(colorsMountainBluebird, 16, "Mountain bluebird"),
                Bird(colorsRobin, 13, "Robin"),
                Bird(colorsAtlanticCanary, 13, "atlantic canary")
        };
            
        
        KNearestNeighbors knn(listb);
        
        vector<Bird> voisins = knn.getKNNDistance(b, 1);

            knn.addNeighbors(b);

            cout << "espèce " << voisins.at(0).getName() << endl;
        }else{
            cout << "pas d'oiseau " << endl;
        }

        


   }

    // Mat ref = imread("../images/A.jpg", IMREAD_COLOR);
    // Mat oiseau = imread("../images/B.jpg", IMREAD_COLOR);


    return 0;

}
 