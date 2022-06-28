#ifndef _KEYFRAME_HH_
#define _KEYFRAME_HH_

#include "Matrix.hh"

namespace Marcel{
  class KeyFrame
  {
  private:
    Matrix Transformation;
    int start;
    int end;
  public:
    KeyFrame(int);
    int setLength(int);
    int addTransformation(Matrix);

    int getStart();
    int getEnd();
    int getLength();
    Matrix getTransformation();
  };
}
#endif
