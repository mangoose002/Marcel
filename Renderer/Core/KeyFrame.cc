using namespace std;
#include "KeyFrame.hh"
#include "Matrix.hh"
#include <iostream>

namespace Marcel{
  KeyFrame::KeyFrame(int s)  {
    start = s;
    Transformation = Matrix(4,4);
    Transformation.Identity();
  }

  int KeyFrame::setLength(int l)  {
    return (end = (start + l)-1);
  }

  int KeyFrame::addTransformation(Matrix m)  {
    Transformation = Transformation * m;
    return 1;
  }

  int KeyFrame::getStart()  {
    return start;
  }

  int KeyFrame::getEnd()  {
    return end;
  }

  Matrix KeyFrame::getTransformation()  {
    return Transformation;
  }

  int KeyFrame::getLength()  {
    return end-start;
  }
}