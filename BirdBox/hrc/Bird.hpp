#ifndef BIRDBOX_BIRD_HPP
#define BIRDBOX_BIRD_HPP

#include <cstdint>
#include <opencv2/core/matx.hpp>

#define NB_COLOR 2

using namespace cv;

class Bird {

private:

    String name;
    double size;
    Vec3b color[NB_COLOR];
    bool isPestBirdSpecies;

public:

    [[nodiscard]] double getSize() const;

    [[nodiscard]] const Vec<uchar, 3> *getColor() const;

    [[nodiscard]] String getName() const;

    void setName( String n);

    Bird(Vec3b color[NB_COLOR], double size, String name="unknow", bool isPestBirdSpecies=false);

    ~Bird();

    [[nodiscard]] bool getIsPestBirdSpecies() const;
};


#endif //BIRDBOX_BIRD_HPP
