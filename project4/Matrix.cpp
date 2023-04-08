#include <complex>
#include "Matrix.h"
#define THRESHOLD 0.1
#define MIN_DIM_SIZE 1
#define LENGTH_ERROR "sizes of matrices don't match for doing this operation"
#define OUT_OF_RANGE "out of range"
#define RUN_TIME_ERROR "problems with the file"
#define STREAM_ERROR "problems with the stream"

// see documentation of functions in header file

Matrix::Matrix (int rows, int cols) : _dim()
{
  if (rows < MIN_DIM_SIZE || cols < MIN_DIM_SIZE)
  {
    throw std::length_error(LENGTH_ERROR);
  }
  _dim.rows = rows;
  _dim.cols = cols;
  _values = new float[rows*cols](); // check it's initialize elements to 0
}

Matrix::Matrix () : Matrix(MIN_DIM_SIZE,MIN_DIM_SIZE) {}


Matrix::Matrix (Matrix const &rhs) : _dim()
{
  _dim = {rhs._dim.rows, rhs._dim.cols};
  _values = new float[_dim.rows*_dim.cols]();
  for (int i=0; i < _dim.rows; i++)
  {
    for(int j=0; j < _dim.cols; j++)
    {
      _values[_dim.cols*i + j] = rhs(i,j);
    }
  }
}

Matrix::~Matrix ()
{
    delete[] _values;
}

float Matrix::operator() (int i, int j) const //need const in arguments?
{
  if (i < 0 || j < 0 || i >= _dim.rows || j >= _dim.cols)
  {
    throw std::out_of_range (OUT_OF_RANGE);
  }
  return _values[_dim.cols * i + j];
}

float& Matrix::operator()(int i, int j)
{
  if (i < 0 || j < 0 || i >= _dim.rows || j >= _dim.cols)
  {
    throw std::out_of_range(OUT_OF_RANGE);
  }
  return _values[_dim.cols*i + j];
}

int Matrix::get_rows() const
{
  return _dim.rows;
}

int Matrix::get_cols() const
{
  return _dim.cols;
}

// Transforms a matrix into its transpose matrix, i.e (A.transpose())ij = Aji.
// Supports function calling concatenation.
Matrix& Matrix::transpose()
{
  // create a new matrix, which is this transpose matrix
  Matrix temp(_dim.cols, _dim.rows);
  for (int i=0; i < temp._dim.rows; i++)
  {
    for (int j=0; j < temp._dim.cols; j++)
    {
      temp(i,j) =  (*this)(j,i);
    }
  }
  // changing this Matrix to be its transpose
  *this = temp;
  return *this;
}

// Transforms a matrix into a column vector. Supports function calling
// concatenation.
Matrix& Matrix::vectorize()
{
  _dim.rows = _dim.rows * _dim.cols;
  _dim.cols = 1;
  return *this;
}

void Matrix::plain_print() const
{
  for (int i=0; i< _dim.rows; i++)
  {
    for (int j=0; j < _dim.cols; j++)
    {
      cout << _values[_dim.cols * i + j] << " ";
    }
    cout << endl;
  }
}

// Returns a matrix which is the elementwise multiplication(Hadamard
// product) of this matrix and another matrix m
Matrix Matrix::dot(Matrix const& rhs) const
{
  if (_dim.rows != rhs._dim.rows || _dim.cols != rhs._dim.cols)
  {
    throw std::length_error(LENGTH_ERROR);
  }
  Matrix m = Matrix(*this);
  for (int i=0; i < _dim.rows; i++)
  {
    for (int j = 0; j < _dim.cols; j++)
    {
      m(i,j) *= rhs(i,j);
    }
  }
  return m;
}

float Matrix::norm() const
{
  float sum = 0;
  for (int i=0; i < _dim.rows; i++)
  {
    for (int j = 0; j < _dim.cols; j++)
    {
      sum += (_values[i * _dim.cols + j])*(_values[i * _dim.cols + j]);
    }
  }
  return std::sqrt (sum); // is it ok to use std::sqrt?
}

Matrix operator+(Matrix const& lhs, Matrix const& rhs)
{
  // have to be from the same dimensions for adding
  if ( lhs._dim.rows != rhs._dim.rows || lhs._dim.cols != rhs._dim.cols)
  {
    throw std::length_error(LENGTH_ERROR);
  }
  Matrix m = Matrix(lhs);
  for (int i=0; i < lhs._dim.rows; i++)
  {
    for (int j=0; j < lhs._dim.cols; j++)
    {
      m(i,j) += rhs(i,j);
    }
  }
  return m;
}

Matrix& Matrix::operator=(Matrix const& rhs)
{
  if (&rhs == this) //the same address - the same instance
  {
    return *this;
  }
  if (_dim.rows != rhs._dim.rows || _dim.cols != rhs._dim.cols)
  {
    delete[] _values;
    _dim = rhs._dim;
    _values = new float[_dim.rows * _dim.cols];
  }
  for (int i = 0; i < _dim.rows; i++)
  {
    for (int j = 0; j < _dim.cols; j++)
    {
      _values[_dim.cols * i + j] = rhs(i,j);
    }
  }
  return *this;
}

Matrix operator*(Matrix const& lhs, Matrix const& rhs)
{
  if (lhs._dim.cols != rhs._dim.rows)
  {
    throw std::length_error(LENGTH_ERROR);
  }
  Matrix m = Matrix(lhs._dim.rows, rhs._dim.cols);
  int rows= m._dim.rows, cols = m._dim.cols;
  for (int i=0; i < rows; i++) // i - the current row we multiply
  {
    for(int j=0; j<cols; j++) // j - the current col we multiply
    {
      float sum = 0;
      for (int k=0; k < lhs._dim.cols; k++) // summing the multiply of the
        // elements in row i and in col j
      {
        sum += lhs(i,k) * rhs(k,j);
      }
      m(i,j) = sum;
    }
  }
  return m;
}

Matrix Matrix::operator*(float s) const
{
  Matrix m = Matrix(*this);
  for (int i=0; i < _dim.rows; i++)
  {
    for (int j=0; j < _dim.cols; j++)
    {
      m(i,j) *= s;
    }
  }
  return m;
}

Matrix operator*(float s, Matrix const& rhs)
{
  return rhs * s;
}

Matrix& Matrix::operator+=(Matrix const& rhs)
{
  if (_dim.rows != rhs._dim.rows || _dim.cols != rhs._dim.cols)
  {
    throw std::length_error(LENGTH_ERROR);
  }
  for (int i=0; i < _dim.rows; i++)
  {
    for (int j=0; j < _dim.cols; j++)
    {
      _values[i*_dim.cols + j] += rhs(i,j);
    }
  }
  return *this;
}


float Matrix::operator[](int i) const
{
  if (i >= _dim.rows * _dim.cols || i < 0)
  {
    throw std::out_of_range(OUT_OF_RANGE);
  }
  return _values[i];
}


float& Matrix::operator[](int i)
{
  if (i >= _dim.rows * _dim.cols || i < 0)
  {
    throw std::out_of_range(OUT_OF_RANGE);
  }
  return _values[i];
}

std::ostream& operator<<(std::ostream& s, Matrix const& rhs)
{
  if (!s)
  {
    throw std::runtime_error (STREAM_ERROR);
  }
  for(int i=0; i < rhs._dim.rows; i++)
  {
    for(int j=0; j < rhs._dim.cols; j++)
    {
      if (rhs(i,j) > THRESHOLD)
      {
        s << "**";
      }
      else
      {
        s << "  ";
      }
    }
    s << endl;
  }
  return s;
}

std::ifstream& operator>>(std::ifstream& s, Matrix& rhs)
{
  if (!s)
  {
    throw std::runtime_error (STREAM_ERROR);
  }
  // buffer to holds the floats in the matrix
  long int buffer_size_bytes = rhs._dim.cols * rhs._dim.rows * sizeof(float);
  char* buffer = new char [buffer_size_bytes];
  // get length of file:
  s.seekg (0, std::ios_base::end);
  long int file_size_bytes = s.tellg();
  if (file_size_bytes != buffer_size_bytes)
  {
    delete[] buffer;
    throw std::length_error(LENGTH_ERROR);
  }
  s.seekg (0, std::ios_base::beg);
  for (int i=0; i < rhs._dim.cols * rhs._dim.rows; i++)
  {
    if (!s.read (buffer, sizeof(float)))
    {
      delete[] buffer;
      throw std::runtime_error (RUN_TIME_ERROR);
    }
    rhs[i] = *((float*)buffer);
  }
  if (!s)
  {
    delete[] buffer;
    throw std::runtime_error(RUN_TIME_ERROR);
  }
  delete[] buffer;
  return s;
}
