#include "../hrc/Bird.hpp"


double Bird::getSize() const {
    return this->size;
}

const Vec3b *Bird::getColor() const {
    return this->color;
}

String Bird::getName() const{
    return this->name;
}

Bird::Bird(Vec3b color[NB_COLOR], double size, String name) : name(name), size(size) {
    std::copy(color, color + NB_COLOR, this->color);
}


Bird::~Bird() {

}
