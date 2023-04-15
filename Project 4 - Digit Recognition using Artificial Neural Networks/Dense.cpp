#include "Dense.h"

// see function documentation in header file

Dense::Dense (Matrix const& weights, Matrix const& bias, active_func
ActivationFunction) :_weights(weights), _bias(bias) , _activation_func
(ActivationFunction) {}

const Matrix& Dense::get_weights() const
{
  return _weights;
}

const Matrix& Dense::get_bias() const
{
  return _bias;
}

const active_func& Dense::get_activation() const
{
  return _activation_func;
}

Matrix Dense::operator()(Matrix const& vector) const // not sure about the
// return val
{
  return _activation_func(_weights * vector + _bias);
}

