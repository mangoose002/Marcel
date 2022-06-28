#ifndef _ROTATION_HH_
#define _ROTATION_HH_

#include "Matrix.hh"
#include "Point.hh"

class Rotation : public Matrix
{
private:
  Point   Center;
  double  Rotx;
  double  Roty;
  double  Rotz;

public:
  Rotation(double x,double y,double z);
};

#endif
