#include "../hrc/KNearestNeighbors.hpp"

KNearestNeighbors::KNearestNeighbors(const vector<Bird>& listBird) {
    for (const auto &bird: listBird) {
        this->listBirdEl.push_back((element) {
            .x=RGBToHexa(bird.getColor()),
            .y=bird.getSize(),
            .b=bird
        });
    }
}

void KNearestNeighbors::addNeighbors(const Bird &bird){
    this->listBirdEl.push_back((element) {
            .x=RGBToHexa(bird.getColor()),
            .y=bird.getSize(),
            .b=bird
    });
}

vector<KNearestNeighbors::element> KNearestNeighbors::getListBirdEl() {
    return this->listBirdEl;
}

/**
 * Permet de convertir une valeur RGB en valeur équivalente en hexadécimal.
 * @param rgb Objet que l'on souhaite convertir.
 * @return    Une valeur hexadécimale en un long non-signé.
 */
unsigned long KNearestNeighbors::RGBToHexa(const Vec3b &rgb) {
    return ((rgb[0] & 0xff) << 16) + ((rgb[1] & 0xff) << 8) + (rgb[2] & 0xff);
}

/**
 * Permet de récupérer la distance entre deux éléments.
 * @param p1 Premier élément servant de point de départ.
 * @param p2 Second élément servant de point d'arrivée.
 * @return   Un double contenant la distance entre les deux éléments.
 */
double KNearestNeighbors::getDistance(const KNearestNeighbors::element &p1, const KNearestNeighbors::element &p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

/**
 * Permet de récupérer une liste des plus proches éléments.
 * @param b Point à partir duquel on veut récupérer les plus proches voisins.
 * @param K Le nombre de voisins que l'on veut récupérer au plus.
 * @return  La liste K éléments les plus proches du point b.
 */
vector<Bird> KNearestNeighbors::getKNN(const Bird& b, int K){
    auto currBird = (element) {
        .x=RGBToHexa(b.getColor()),
        .y=b.getSize(),
        .b=b
    };

    sort(this->listBirdEl.begin(), this->listBirdEl.end(),
         [currBird](const element &e1, const element &e2) {
             return getDistance(e1, currBird) < getDistance(e2, currBird);
         }
    );

    vector<Bird> result;
    for_each(this->listBirdEl.begin(), this->listBirdEl.end(),
         [&](KNearestNeighbors::element &e) {
             if (result.size() < K)
                 result.push_back(e.b);
         }
    );

    if (result.empty())
        throw domain_error("Il n'existe pas de voisins.");

    return result;
}

/**
 * Permet d'obtenir l'élément le plus récurent dans une liste.
 * @param listBird La liste dans laquelle on souhaite regarder les éléments les plus fréquents.
 * @return         Le nom de l'élément le plus courant.
 */
String KNearestNeighbors::getMostCommonType(const vector<Bird>& listBird){
    map<String, int> names;
    for (auto &i: listBird) {
        if (names[i.getName()]) names[i.getName()] += 1;
        else names[i.getName()] = 1;
    }

    int tmp_max = 0;
    String tmp_res;
    for (const auto &[key, value]: names) {
        if (value > tmp_max) {
            tmp_max = value;
            tmp_res = key;
        }
    }

    return tmp_res;
}
