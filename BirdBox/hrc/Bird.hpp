#ifndef BIRDBOX_BIRD_HPP
#define BIRDBOX_BIRD_HPP

#include <cstdint>
#include <opencv2/core/matx.hpp>

using namespace cv;

class Bird {

private:

    String name;
    double size;
    Vec3b color;

public:

    [[nodiscard]] double getSize() const;

    Vec3b &getColor();

    String getName();

    Bird(String name, const Vec3b& color, double size);

    ~Bird();

};


#endif //BIRDBOX_BIRD_HPP
