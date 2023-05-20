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

    [[nodiscard]] Vec3b getColor() const;

    [[nodiscard]] String getName() const;

    void setName( String n);

    Bird(const Vec3b& color, double size, String name="unknow");

    ~Bird();

};


#endif //BIRDBOX_BIRD_HPP
