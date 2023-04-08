#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"
#include <cmath>
using std::exp;

/**
 * activation function of neural network
 */
namespace activation
{
    /**
     * create a new Matrix from m Matrix which it's element in values
     * attribute is x, and the new Matrix element will be Relu(x)
     * ∀x ∈ R ReLU(x) = (x <- x ≥ 0 , 0 <- else)
     * @param m is the Matrix whith the x elements
     * @return the new Matrix
     */
    Matrix relu(Matrix const& m);

    /**
     * create a new Matrix which is the distribution Matrix of m
     * @param m is the Matrix to create from it's values a distribution Matrix
     * @return
     */
    Matrix softmax(Matrix const& m);
}

#endif //ACTIVATION_H