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

void Bird::setName(String n){
    this->name=n;
}

bool Bird::getIsPestBirdSpecies() const{
    return this->isPestBirdSpecies;
}

Bird::Bird(Vec3b color[NB_COLOR], double size, String name, bool isPestBirdSpecies) : name(name), size(size), isPestBirdSpecies(isPestBirdSpecies) {
    std::copy(color, color + NB_COLOR, this->color);
}

Bird::~Bird() {

}
