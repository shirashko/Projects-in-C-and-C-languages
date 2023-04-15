#include "Activation.h"
#define ZERO_ERROR "can't divide something with zero"

Matrix activation::relu(Matrix const& input)
{
  Matrix output(input);
  for (int i=0; i < input.get_rows(); i++)
  {
    for (int j=0; j < input.get_cols(); j++)
    {
      if (input (i, j) < 0)
      {
        output (i, j) = 0;
      }
    }
  }
  return output;
}

Matrix activation::softmax(Matrix const& input)
{
  float sum = 0;
  Matrix output = Matrix(input.get_rows(), input.get_cols());
  for (int i=0; i < output.get_rows(); i++)
  {
    for (int j=0; j < output.get_cols(); j++)
    {
      sum += exp (input(i,j));
      output(i,j) = exp (input(i,j));
    }
  }
  if (sum == 0)
  {
    throw std::runtime_error(ZERO_ERROR);
  }
  return (1/sum) * output;
}