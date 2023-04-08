#ifndef DENSE_H
#define DENSE_H
#include "Activation.h"

// this is a pointer to an activation function
typedef Matrix (*active_func)(Matrix const& m);

/**
 * class Dense represent a layer in neuron network. weights is for the
 * connections of the neurons in the layer, bias is for normalize to the
 * values we want to get, and activation function is the function that
 * create the output of this layer, which is the input of the next layer /
 * the output of the whole network
 */
class Dense
{
 private:
  Matrix _weights;
  Matrix _bias;
  active_func _activation_func;
 public:

  /**
   * construct a Dense instance
   * @param weights
   * @param bias
   * @param ActivationFunction
   */
  Dense(Matrix const& weights, Matrix const& bias, active_func
  ActivationFunction);

  /**
   * get the weights of this Dense instance
   * @return the weights of this Dense instance
   */
  const Matrix& get_weights() const;

  /**
   * get the bias of this Dense instance
   * @return the bias of this Dense instance
   */
  const Matrix& get_bias() const;

  /**
   * get the activation function of this Dense instance
   * @return this activation function
   */
  const active_func& get_activation() const;

  /**
   * Applies the layer on input and returns output matrix
   * @param m is the Matrix which represent the input of this Dense instance
   * @return Matrix instance which is the output of this layer given this input
   */
  Matrix operator()(Matrix const& m) const;
};



#endif //DENSE_H
