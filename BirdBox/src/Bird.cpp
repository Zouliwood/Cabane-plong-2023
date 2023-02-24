#include "../hrc/Bird.hpp"


double Bird::getSize() const {
    return this->size;
}

Vec3b &Bird::getColor() {
    return this->color;
}

String Bird::getName() {
    return this->name;
}

Bird::Bird(String name, const Vec3b& color, double size) : name{name}, color{color}, size{size} {}

Bird::~Bird() {

}
