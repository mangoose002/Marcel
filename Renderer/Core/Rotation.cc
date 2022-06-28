using namespace std;
#include "Rotation.hh"
#include "Point.hh"
#include <math.h>

Rotation::Rotation(double rotx, double roty,double rotz):Matrix(4,4)
{
  //  Center = C;
  Rotx   = 2*(rotx/360.0)*M_PI;
  Roty   = 2*(roty/360.0)*M_PI;
  Rotz   = 2*(rotz/360.0)*M_PI;

  Matrix RotXm(4,4);
  Matrix RotYm(4,4);
  Matrix RotZm(4,4);
  Matrix Rot(4,4);

  RotZm.setValue(0,0,cos(Rotz));
  RotZm.setValue(0,1,-sin(Rotz));
  RotZm.setValue(1,0,sin(Rotz));
  RotZm.setValue(1,1,cos(Rotz));
  RotZm.setValue(2,2,1);
  RotZm.setValue(3,3,1);

  RotYm.setValue(0,0,cos(Roty));
  RotYm.setValue(0,2,sin(Roty));
  RotYm.setValue(2,0,-sin(Roty));
  RotYm.setValue(2,2,cos(Roty));
  RotYm.setValue(1,1,1);
  RotYm.setValue(3,3,1);

  RotXm.setValue(1,1,cos(Rotx));
  RotXm.setValue(1,2,sin(Rotx));
  RotXm.setValue(2,1,-sin(Rotx));
  RotXm.setValue(2,2,cos(Rotx));
  RotXm.setValue(0,0,1);
  RotXm.setValue(3,3,1);

  Rot = RotZm * RotYm * RotXm;

  for(int i=0;i<isize;i++)
    for(int j=0;j<jsize;j++)
      table[i][j]=Rot.getValue(i,j);
}
