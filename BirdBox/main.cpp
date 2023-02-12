#include "hrc/Calcul.hpp"




int main(int argc, char **argv) {

    Mat el1 = imread("../images/bird.jpg", IMREAD_GRAYSCALE);
    Mat el2 = imread("../images/bird.jpg", IMREAD_GRAYSCALE);
    float szObject = Calcul::sizeObject(el1, el2);
    cout << "sizeObject: " << szObject << endl;

    destroyAllWindows();

    return 0;

}
