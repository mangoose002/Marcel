#ifndef _MATRIX_HH_
#define _MATRIX_HH_

#include <iostream>
#include "Core/Vector.hh"
#include "Core/Point.hh"

namespace Marcel{
  class Matrix
  {
  protected:
    double **table;
    int isize;
    int jsize;
  public:
    Matrix();
    Matrix(int,int);
    ~Matrix();
    Matrix(const Matrix&);

    void   Identity();
    double getValue(int i,int j);
    int    getIsize();
    int    getJsize();
    void setValue(int i,int j, double v);

    Matrix operator+(Matrix);
    Matrix operator-(Matrix);

    Matrix operator*(Matrix);
    Matrix operator*(double);
    Vector operator*(Vector);
    Point  operator*(Point);

    int operator=(Matrix);
  };

  std::ostream& operator<<(std::ostream& o,Matrix);
}
#endif
