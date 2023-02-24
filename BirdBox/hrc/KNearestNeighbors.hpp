#ifndef BIRDBOX_KNEARESTNEIGHBORS_HPP
#define BIRDBOX_KNEARESTNEIGHBORS_HPP


#include <vector>
#include "Bird.hpp"
#include <map>

using namespace std;

class KNearestNeighbors {

private:

    struct element {
        unsigned long x{};
        double y{};
        Bird b;
    };

    static KNearestNeighbors instance;

    vector<element> listBirdEl;


    explicit KNearestNeighbors(vector<Bird> listBird);

    ~KNearestNeighbors() = default;


    static unsigned long RGBToHexa(Vec3b &rgb);

    static double getDistance(const element &p1, const element &p2);

public:

    static KNearestNeighbors &getInstance();

    KNearestNeighbors(const KNearestNeighbors &) = delete;

    String getMostCommonType(vector<Bird> listEl);

    [[nodiscard]] vector<Bird> getKNN(Bird b, int K);

};


#endif //BIRDBOX_KNEARESTNEIGHBORS_HPP
