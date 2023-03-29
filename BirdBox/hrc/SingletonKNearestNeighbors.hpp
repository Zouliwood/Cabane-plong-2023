#ifndef BIRDBOX_SINGLETONKNEARESTNEIGHBORS_HPP
#define BIRDBOX_SINGLETONKNEARESTNEIGHBORS_HPP

#include "../hrc/KNearestNeighbors.hpp"

class SingletonKNearestNeighbors {

private:

    static KNearestNeighbors instance;

public:

    static KNearestNeighbors &getInstance();

};


#endif //BIRDBOX_SINGLETONKNEARESTNEIGHBORS_HPP
