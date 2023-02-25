#include "../hrc/SingletonKNearestNeighbors.hpp"


KNearestNeighbors SingletonKNearestNeighbors::instance = KNearestNeighbors(vector<Bird>({
    //TODO: load data
}));

KNearestNeighbors &SingletonKNearestNeighbors::getInstance() {
    return instance;
}
