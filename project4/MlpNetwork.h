#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

/**
 * represent a neuron network
 */
class MlpNetwork
{
 private:
  Dense _layer1;
  Dense _layer2;
  Dense _layer3;
  Dense _layer4;
 public:

 /**
 * construct a MlpNetwork
 * @param weights is an array of 4 Matrices which represent the weights which
  * needed to be put as the weights attribute of this instance layers
  * respectively to their indexes
 * @param biases is an array of 4 biases Matrices which represent the biases
  * which needed to be put as the weights attribute of this instance layers
  * respectively to their indexes
 */
  MlpNetwork(Matrix weights[],Matrix biases[]);

  /**
   * Applies the entire network on input. returns digit struct.
   * @param m is the input to this network, represent the image needed to be
   * interpreted
   * @return digit struct which represent the value this network concluded the 
   * input represent, and the probability of this conclusion is right
   */
  digit operator()(Matrix const& m) const;
};


#endif // MLPNETWORK_H