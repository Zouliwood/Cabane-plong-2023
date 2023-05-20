#include "../hrc/KNearestNeighbors.hpp"

KNearestNeighbors::KNearestNeighbors(const vector<Bird> &listBird) {
    for (const auto &bird: listBird) {
        this->listBirdEl.push_back((element) {
                .xHex=RGBToHexa(bird.getColor()),
                .xRGB=bird.getColor(),
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
 * @param p1            Premier élément servant de point de départ.
 * @param p2            Second élément servant de point d'arrivée.
 * @param typeDistance  Méthode utilisée pour calculer la distance.
 * @param typeColor     Couleur utilisée pour le calcul.
 * @return              Un double contenant la distance entre les deux éléments.
 */
double KNearestNeighbors::getDistance(const KNearestNeighbors::element &p1, const KNearestNeighbors::element &p2,
                                      int typeDistance, int typeColor) {

    if (typeDistance == COSINE)
        return getDistanceCosine(p1, p2, typeColor);
    else if (typeDistance == EUCLIDEAN)
        return getDistanceEuclidean(p1, p2, typeColor);
    else /*if (typeDistance==MANHATTAN)*/
        return getDistanceManhattan(p1, p2, typeColor);

}

double KNearestNeighbors::getDistanceCosine(const KNearestNeighbors::element &p1,
                                            const KNearestNeighbors::element &p2, int type) {

    double size = pow(p2.y - p1.y, 2);
    double magnitudep1 = sqrt(pow(p1.xRGB[0], 2) + pow(p1.xRGB[1], 2) + pow(p1.xRGB[2], 2));
    double magnitudep2 = sqrt(pow(p2.xRGB[0], 2) + pow(p2.xRGB[1], 2) + pow(p2.xRGB[2], 2));
    double cosineSimilarity;
    if (type == COSINE_NORMALIZED) {

        double colorP1 = p1.xRGB[0] + p1.xRGB[1] + p1.xRGB[2];
        double colorP2 = p2.xRGB[0] + p2.xRGB[1] + p2.xRGB[2];
        cosineSimilarity = (p1.xRGB[0] * p2.xRGB[0] + p1.xRGB[1] * p2.xRGB[1] + p1.xRGB[2] * p2.xRGB[2]) /
                           ((magnitudep1 / colorP1) * (magnitudep2 / colorP2));

    } else if (type == COSINE) {

        cosineSimilarity = (p1.xRGB[0] * p2.xRGB[0] + p1.xRGB[1] * p2.xRGB[1] + p1.xRGB[2] * p2.xRGB[2]) /
                           (magnitudep1 * magnitudep2);

    }
    return sqrt((1 - cosineSimilarity) + size);

}

double KNearestNeighbors::getDistanceEuclidean(const KNearestNeighbors::element &p1,
                                               const KNearestNeighbors::element &p2, int type) {

    double size = pow(p2.y - p1.y, 2);
    if (type == HEX)
        return sqrt(pow(p2.xHex - p1.xHex, 2) + size);
    else if (type == RGB_NORMALIZED) {
        double colorP1 = p1.xRGB[0] + p1.xRGB[1] + p1.xRGB[2];
        double colorP2 = p2.xRGB[0] + p2.xRGB[1] + p2.xRGB[2];

        double normalizedP1[3] = {p1.xRGB[0] / colorP1, p1.xRGB[1] / colorP1, p1.xRGB[2] / colorP1};
        double normalizedP2[3] = {p2.xRGB[0] / colorP2, p2.xRGB[1] / colorP2, p2.xRGB[2] / colorP2};

        double resNor = size;
        for (int i = 0; i < 3; ++i)
            resNor += pow(normalizedP2[i] - normalizedP1[i], 2);

        return resNor;
    } else /*if (type == RGB) */{
        double res = size;
        for (int i = 0; i < 3; ++i)
            res += pow(p2.xRGB[i] - p1.xRGB[i], 2);
        return sqrt(res);
    }

}

double KNearestNeighbors::getDistanceManhattan(const KNearestNeighbors::element &p1,
                                               const KNearestNeighbors::element &p2, int type) {

    double size = fabs(p2.y - p1.y);
    if (type == HEX)
        return fabs(p2.xHex - p1.xHex) + size;
    else if (type == RGB_NORMALIZED) {
        double color_p1 = p1.xRGB[0] + p1.xRGB[1] + p1.xRGB[2];
        double color_p2 = p2.xRGB[0] + p2.xRGB[1] + p2.xRGB[2];

        double normalized_p1[3] = {p1.xRGB[0] / color_p1, p1.xRGB[1] / color_p1, p1.xRGB[2] / color_p1};
        double normalized_p2[3] = {p2.xRGB[0] / color_p2, p2.xRGB[1] / color_p2, p2.xRGB[2] / color_p2};

        double res = size;
        for (int i = 0; i < 3; ++i)
            res += fabs(normalized_p2[i] - normalized_p1[i]);

        return sqrt(res);
    } else /*if (type == RGB) */{
        double res = size;
        for (int i = 0; i < 3; ++i)
            res += fabs(p2.xRGB[i] - p1.xRGB[i]);
        return sqrt(res);
    }

}

/**
 * Permet de récupérer une liste des plus proches éléments.
 * @param b Point à partir duquel on veut récupérer les plus proches voisins.
 * @param K Le nombre de voisins que l'on veut récupérer au plus.
 * @return  La liste K éléments les plus proches du point b.
 */
vector<Bird> KNearestNeighbors::getKNNDistance(const Bird &b, int K, int type, int typeColor) {
    auto currBird = (element) {
            .xHex=RGBToHexa(b.getColor()),
            .xRGB=b.getColor(),
            .y=b.getSize(),
            .b=b
    };

    sort(this->listBirdEl.begin(), this->listBirdEl.end(),
         [currBird, &type, &typeColor](const element &e1, const element &e2) {
             return getDistance(e1, currBird, type, typeColor) < getDistance(e2, currBird, type, typeColor);
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
 * Permet de récupérer une liste des plus proches éléments en fonction de la taille de la fenêtre.
 * @param b Point à partir duquel on veut récupérer les plus proches voisins.
 * @param K Le nombre de voisins que l'on veut récupérer au plus.
 * @return  La liste d'éléments visibles dans la fenêtre.
 */
vector<Bird> KNearestNeighbors::getKNNWindow(const Bird &b, int K, int type, int typeColor) {
    auto currBird = (element) {
            .xHex=RGBToHexa(b.getColor()),
            .xRGB=b.getColor(),
            .y=b.getSize(),
            .b=b
    };

    sort(this->listBirdEl.begin(), this->listBirdEl.end(),
         [currBird, &type, &typeColor](const element &e1, const element &e2) {
             return getDistance(e1, currBird, type, typeColor) < getDistance(e2, currBird, type, typeColor);
         }
    );

    vector<element> listKElement;
    for_each(this->listBirdEl.begin(), this->listBirdEl.end(),
             [&](KNearestNeighbors::element &e) {
                 if (listKElement.size() < K)
                     listKElement.push_back(e);
             }
    );

    auto N = double(this->listBirdEl.size());
    double window = K / (2 * N * getDistance(listKElement.back(), currBird, type, typeColor));

    vector<Bird> result;
    for_each(listKElement.begin(), listKElement.end(),
             [&](KNearestNeighbors::element &e) {
                 if (getDistance(e, currBird, type, typeColor) <= window)
                     result.push_back(e.b);
             }
    );

    return result;
}

/**
 * Permet d'obtenir l'élément le plus récurent dans une liste.
 * @param listBird La liste dans laquelle on souhaite regarder les éléments les plus fréquents.
 * @return         Le nom de l'élément le plus courant.
 */
String KNearestNeighbors::getMostCommonType(const vector<Bird> &listBird) {
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
