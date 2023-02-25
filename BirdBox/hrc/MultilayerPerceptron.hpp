#ifndef BIRDBOX_MULTILAYERPERCEPTRON_HPP
#define BIRDBOX_MULTILAYERPERCEPTRON_HPP

#include <vector>
#include <valarray>
#include "Matrix.hpp"

#define SIGMOID [](double v){return 1/(1+exp(-v));}
#define DERIVATE_SIGMOID [](double v){return (v*(1-v));}
#define LINEAR [](double v){return v;}
#define THRESHOLD [](double v){return (v>=0)?1:0);}

using namespace std;

class MultilayerPerceptron {

private:

    struct Layer {
        Perceptron biasActivation;
        Perceptron weightPaths;
    };

    size_t input, output, middle;
    vector<Layer> layers;
    float learningRate;

public:

    MultilayerPerceptron(size_t middle, size_t input, size_t output, size_t nbrLayerMiddle, float learningRate);

    Perceptron forward(Perceptron &p);

    void backward(Perceptron &p);

};


#endif //BIRDBOX_MULTILAYERPERCEPTRON_HPP
