#include "../hrc/Bird.hpp"


double Bird::getSize() const {
    return this->size;
}

Vec3b Bird::getColor() const{
    return this->color;
}

String Bird::getName() const{
    return this->name;
}

Bird::Bird::Bird(const Vec3b &color, double size, String name)  : name{name}, color{color}, size{size} {}

Bird::~Bird() {

}
