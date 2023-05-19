#include "hrc/Calcul.hpp"
#include "hrc/Couleur.hpp"
#include "hrc/Bird.hpp"
#include "hrc/KNearestNeighbors.hpp"

//ip raspberry : 192.168.1.10


using namespace cv; 


int main(int argc, char **argv) {

   while(1){
        Mat ref=Camera::getPic("../images/src_cabane.jpg");
        sleep(5);
        Mat oiseau=Camera::getPic("../images/oiseau.jpg");

        Mat t = Image::msq( oiseau,  ref);

        //Mat croppedImage= Image::cropImg(t,oiseau);

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

        Rect bdRect = boundingRect(contours[largestContourIndex]);

        

        Mat croppedImage = image_cpy(bdRect);

        imwrite("../images/cropped_image.jpg", croppedImage);

        Vec3b domColor = Couleur::MostFrequentColor(croppedImage);

        cout << "couleur dominante : " << domColor << endl;

         //algo knn

        Bird b(domColor,sizeBird,"unknown");


        vector<Bird> listb{Bird(Vec3b(255, 55, 0), 45, "Cockatiel"),
                Bird(Vec3b(255, 55, 0), 50, "Triton cockatoo"),
                Bird(Vec3b(255, 0, 0), 15, "Gouldian finch"),
                Bird(Vec3b(86, 178, 233), 16, "Mountain bluebird"),
                Bird(Vec3b(201, 162, 57), 13, "Robin"),
                Bird(Vec3b(225, 225, 51), 13, "atlantic canary"),
                Bird(Vec3b(65, 105, 225), 25, "Blue Jay"),
                Bird(Vec3b(255, 245, 250), 35, "Columba")};
            
        
        KNearestNeighbors knn(listb);
        
        vector<Bird> voisins = knn.getKNN(b, 1);

        b.setName(voisins.at(0).getName());

        knn.addNeighbors(b);

        cout << "espèce " << voisins.at(0).getName() << endl;


   }

    // Mat ref = imread("../images/A.jpg", IMREAD_COLOR);
    // Mat oiseau = imread("../images/B.jpg", IMREAD_COLOR);


    return 0;

}
 