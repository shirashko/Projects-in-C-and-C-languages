#include "MlpNetwork.h"
#define LENGTH_ERROR "invalid size of matrix"
// ok to do this in this way? need to check validity of arguments?
MlpNetwork::MlpNetwork(Matrix weights[],Matrix biases[]) : _layer1
(weights[0], biases[0], activation::relu) ,
_layer2 (weights[1], biases[1],activation::relu) ,
_layer3(weights[2],biases[2],activation::relu),
_layer4(weights[3],biases[3],activation::softmax)
{
  for (int i = 0; i < 4; i++)
  {
    if (weights[i].get_cols () != weights_dims[i].cols ||
        weights[i].get_rows () != weights_dims[i].rows)
    {
      throw std::length_error (LENGTH_ERROR);
    }
    if (biases[i].get_cols () != bias_dims[i].cols ||
        biases[i].get_rows () != bias_dims[i].rows)
    {
      throw std::length_error (LENGTH_ERROR);
    }
  }  // need to check size of args is really 4?
} // need to check validity of input?

digit MlpNetwork::operator()(Matrix const& matrix) const
{
  // need to check validity of input?
  Matrix output =  _layer4(_layer3(_layer2(_layer1(matrix))));
  float max = 0;
  unsigned int index;
  for (int i=0; i < output.get_rows() * output.get_cols(); i++)
  {
    if (output[i] > max)
    {
      max = output[i];
      index = i;
    }
  }
  digit d = {index, max};
  return d;
}