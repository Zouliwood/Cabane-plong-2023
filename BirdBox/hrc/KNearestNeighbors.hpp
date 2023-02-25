#ifndef BIRDBOX_KNEARESTNEIGHBORS_HPP
#define BIRDBOX_KNEARESTNEIGHBORS_HPP


#include <vector>
#include "Bird.hpp"
#include <map>
#include <stdexcept>

using namespace std;

class KNearestNeighbors {

public:

    struct element {
        unsigned long x{};
        double y{};
        Bird b;
    };

    vector<element> getListBirdEl();

    ~KNearestNeighbors() = default;

    KNearestNeighbors(const KNearestNeighbors &) = delete;

    static String getMostCommonType(const vector<Bird>& listEl);

    [[nodiscard]] vector<Bird> getKNNDistance(const Bird& b, int K);

    [[nodiscard]] vector<Bird> getKNNWindow(const Bird& b, int K);

    explicit KNearestNeighbors(const vector<Bird>& listBird);

private:

    vector<element> listBirdEl;

    static unsigned long RGBToHexa(const Vec3b &rgb);

    static double getDistance(const element &p1, const element &p2);

};


#endif //BIRDBOX_KNEARESTNEIGHBORS_HPP
