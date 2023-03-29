#include "../hrc/MultilayerPerceptron.hpp"

MultilayerPerceptron::MultilayerPerceptron(
        size_t nbrMiddle, size_t nbrInput, size_t nbrOutput, size_t nbrLyrMid, float learningRate
) : input{nbrInput}, output{nbrOutput}, middle{nbrMiddle}, learningRate{learningRate} {

    for (int i = 0; i < nbrLyrMid - 1; ++i) {
        layers.push_back((Layer) {
                .biasActivation = Perceptron((i == nbrLyrMid - 1) ? nbrOutput : nbrMiddle, 1),
                .weightPaths = Perceptron((i == nbrLyrMid - 1) ? nbrOutput : nbrMiddle, (i == 0) ? nbrInput : nbrMiddle)
        });
    }

}

/**
 * WIP : Calculer la propagation avant du réseau en appliquant la fonction d'activation à chaque couche.
 * @param   p Paramètres à partir duquel on veut calculer la sortie du réseau.
 * @return    La valeur de la dernière couche du réseau.
 */
Perceptron MultilayerPerceptron::forward(Perceptron &p) {
    if (p.getX() != input)
        throw invalid_argument("Incompatible matrix dimensions");

    //TODO: WIP

    return Perceptron();
}

/**
 * WIP : Calculer la rétropropagation du gradient pour ajuster la valeur des poids et des biais.
 * @param p Paramètres à partir duquel on veut calculer la rétropropagation.
 */
void MultilayerPerceptron::backward(Perceptron &p) {
    if (p.getX() != this->output)
        throw invalid_argument("Incompatible matrix dimensions");

    //TODO: WIP

}
