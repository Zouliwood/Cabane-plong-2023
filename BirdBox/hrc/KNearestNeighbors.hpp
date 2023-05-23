#ifndef BIRDBOX_KNEARESTNEIGHBORS_HPP
#define BIRDBOX_KNEARESTNEIGHBORS_HPP


#include <vector>
#include "Bird.hpp"
#include <map>
#include <stdexcept>
#include <iostream>

#define EUCLIDEAN 1
#define MANHATTAN 0
#define COSINE 3

#define COSINE_NORMALIZED 4
#define COSINE 3
#define RGB_NORMALIZED 2
#define RGB 1
#define HEX 0

#define NB_COLOR 2

using namespace std;

class KNearestNeighbors {

public:

    struct element {
        unsigned long xHex{};
        const Vec<uchar, 3>* xRGB;
        double y{};
        Bird b;
    };

    vector<element> getListBirdEl();

    ~KNearestNeighbors() = default;

    KNearestNeighbors(const KNearestNeighbors &) = delete;

    void addNeighbors(const Bird &bird);

    static String getMostCommonType(const vector<Bird>& listEl);

    [[nodiscard]] vector<Bird> getKNNDistance(const Bird& b, int K, int type = EUCLIDEAN, int typeColor = RGB);

    [[nodiscard]] vector<Bird> getKNNWindow(const Bird& b, int K, int type = EUCLIDEAN, int typeColor = RGB);

    explicit KNearestNeighbors(const vector<Bird>& listBird);

private:

    vector<element> listBirdEl;     

        

    static unsigned long RGBToHexa(const Vec3b &rgb);

    //static unsigned long RGBTo

    static double getDistanceEuclidean(const element &p1, const element &p2, int type = RGB);

    static double getDistanceManhattan(const element &p1, const element &p2, int type = RGB);

    static double getDistanceCosine(const element &p1, const element &p2, int type = COSINE);

    static double getDistance(const element &p1, const element &p2, int type = EUCLIDEAN, int typeColor = RGB);
};


#endif //BIRDBOX_KNEARESTNEIGHBORS_HPP
