#ifndef BIRDBOX_CAMERA_HPP
#define BIRDBOX_CAMERA_HPP

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

class Camera {

private:

    const float lnBase, alpha;

    static const Camera instance;

    explicit Camera(float baseline = 0, float alpha = 0);

public:

    [[nodiscard]] float getAlpha() const;

    [[nodiscard]] float getLnBase() const;

    Camera(const Camera &) = delete;

    static Mat getPic(int n);

    static const Camera &getUniqueInstance();

    [[nodiscard]] float getDepth(const Point &pointLeft, const Point &pointRight, const Mat &imageRight) const;

};

#endif //BIRDBOX_CAMERA_HPP
