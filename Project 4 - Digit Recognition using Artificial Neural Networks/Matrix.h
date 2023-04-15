// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <cstring> // allowed?
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

/**
 * class Matrix
 * @brief Matrix instance with dimensions, values and functions and
 * operators that support classic operations on matrix \ matrices
 */
class Matrix
{
 private:
    matrix_dims _dim;
    float * _values;
 public:
  /**
   * construct a new Matrix instance, dimension rows X cols, inits all elements
   * to zero
   * @param rows - number of rows in this matrix
   * @param cols - number of cols in this matrix
   */
  Matrix(int rows, int cols);

  /**
   * construct a new Matrix instance of size 1×1, Inits the single element to 0
   */
  Matrix();

  /**
   * copy constructor - construct a new Matrix instance from another Matrix
   * rhs, with the same values (deep copy)
   * @param rhs the Matrix instance to copy from
   */
  Matrix(Matrix const &rhs);

  /**
   * destructor of Matrix instances
   */
  ~Matrix();

  /**
   * get number of rows in this Matrix instance
   * @return number of rows
   */
  int get_rows() const;

  /**
 * get number of cols in this Matrix instance
 * @return number of cols
 */
  int get_cols() const;

  /**
   * Transforms a matrix into its transpose matrix, i.e (A.transpose())ij =
   * Aji. Supports function calling concatenation
   * @return the transpose Matrix
   */
  Matrix& transpose();

  /**
   * Transforms a matrix into a column vector. Supports function calling
   * concatenation.
   * @return this Matrix (now represented as a vector)
   */
  Matrix& vectorize();

  /**
   * Prints matrix elements
   */
  void plain_print() const;

  /**
   * Returns a matrix which is the elementwise multiplication(Hadamard
   * product) of this matrix and another matrix rhs: ∀i, j : (A.dot(B))ij =
   * Aij · Bij
   * @param rhs - the Matrix to multiply the elements with
   * @return the new Matrix which is the Hadamard product
   */
  Matrix dot(Matrix const& rhs) const;

  /**
   * @return the Frobenius norm of the given Matrix
   */
  float norm() const; //check what should be the return value

  /**
   * create a new Matrix instance which it's values is the addition of
   * elements from lhs Matrix and rhs Matrix
   * @param lhs - Matrix from which to take the values and add the other
   * @param rhs - Matrix from which to take the values and add the other
   * @return new Matrix instance which is the addition of lhs and rhs
   */
  friend Matrix operator+(Matrix const& lhs, Matrix const& rhs);
  // check if suppose to do friend or class function. should I document in c
  // file since it is a friend function?

  /**
   * changing this to Matrix with the values of rhs Matrix
   * @param rhs the Matrix to copy from (deep copy)
   * @return this Matrix (with the new values)
   */

  Matrix& operator=(Matrix const& rhs);

  /**
   * create a new Matrix which is the multiply of lhs and rhs Matrix instances
   * @param lhs the matrix to multiply with the other
   * @param rhs the matrix to multiply with the other
   * @return the new Martix which is the multiply result
   */
  friend Matrix operator*(Matrix const& lhs, Matrix const& rhs); //check
  // if suppose to do friend with two arguments

  /**
   * multiply this with the number s
   * @param s the number to multiply with
   * @return new Matrix instance which is the multiply result
   */
  Matrix operator*(float s) const;

  /**
   * multiply rhs with the number s
   * @param s the number to multiply with the given Matrix
   * @param rhs the Matrix to multiply
   * @return new Matrix instance which is the multiply result
   */
  friend Matrix operator*(float s, Matrix const& rhs); //check if
  // that's what they meant

  /**
   * add rhs values to this values. (A)ij += (B)ij for every element
   * rhs need to be with the same dimensions as this instance
   * @param rhs the Matrix instance to add it's values to this Matrix instance
   * @return this Matrix (with it's new values)
   */
  Matrix& operator+=(Matrix const& rhs);

  /**
   * For i,j indices, Matrix m: m(i,j) will return the i,j element in the
   * matrix
   * @param i - number of row
   * @param j - number of column
   * @return this _values element in row i and column j
   */
  float operator()(int i, int j) const;

  /**
   * For i,j indices, Matrix m: m(i,j) will return the i,j element in the
   * matrix
   * @param i - number of row
   * @param j - number of column
   * @return reference to this _values element in row i and column j
   */
  float& operator()(int i, int j);

  /**
   *
   * @param i
   * @return
   */
  float operator[](int i) const;

  /**
   *For i index, Matrix m: m[i] will return the i'th element
   * the purpose is to enable access with only one index
   * @param i is the place in memory of _values needed to be return
   * @return the element of this _values Matrix in the i'th place
   */
  float& operator[](int i);

  /**
   * export/print the image/knowledge that this Matrix represent
   * @param s is the stream to export to
   * @param rhs is the Matrix instance to export it's value
   * @return the given stream
   */
  friend std::ostream& operator<<(std::ostream& s, Matrix const& rhs);

  /**
   * Fills matrix elements
   * has to read input stream fully, otherwise it's an error
   * @param s the stream to get the input from
   * @param rhs the Matrix instance to put the input in
   * @return the given stream
   */
  friend std::ifstream& operator>>(std::ifstream& s, Matrix& rhs);
};


#endif //MATRIX_H