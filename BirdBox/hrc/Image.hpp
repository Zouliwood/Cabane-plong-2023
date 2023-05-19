#ifndef BIRDBOX_IMAGE_HPP
#define BIRDBOX_IMAGE_HPP

#include <opencv2/opencv.hpp>
#include <thread>
#include <unistd.h>
#include <functional>

using namespace cv;
using namespace std;

class Image {

private:

    static void run(const Mat &image, function<void(int, int, int)> &&lambda, int rayon = 0);

public:

    Image(const Image &) = delete;

    static void morphGradient(const Mat &image, Mat &result, int rayon = 5);

    static void masque(const Mat &image, const Mat &background, Mat &result, int seuil = 5);
    static void masquecolor(const Mat &image, const Mat &background, Mat &result, int seuil); 
    static void cropImage( Mat &image);

   static Mat cropImg(Mat &image_cpy, Mat &oiseau);


    static Mat msq(Mat oiseau, Mat ref);

    static void bnry(Mat thresh);


    static vector<Point> addContours(Mat thresh, Mat image_cpy);

    static void drawCross(Mat &image, Point &point, int rayon = 10);

    static float getPxSizeObject(const Mat &masque);

    [[nodiscard]] static Point getObject(Mat &image, const Mat &masque);

    static vector<vector<Point>> getBordures(const Mat &masque);

    static Mat getDifferenceImage(const Mat& image1, const Mat& image2);
};

#endif //BIRDBOX_IMAGE_HPP
