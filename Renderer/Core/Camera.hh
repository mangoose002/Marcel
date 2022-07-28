#ifndef _CAMERA_HH_
#define _CAMERA_HH_

#include "Core/Point.hh"
#include "Core/Plan.hh"
#include "Core/Vector.hh"
//#include "Core/Object.hh"

namespace Marcel{

class Camera
{
private:
  Point   Target;
  Point   ViewPoint;
  Point   ViewPlaneOrigin;
  Plan    ViewPlane;
  Vector  ViewDirection;
  Vector  X;
  Vector  Y;
  int     XResolution;
  int     YResolution;
  double  focus;
  bool    _isGenerated;

  void getViewPlaneCoordinatesFromPoint(Point,int*,int*);
  void getPointFromViewPlaneCoordinates(Point *,float,float);

  public:
  Camera();
  Camera(Point,Point,double,int,int);
  ~Camera();

  Point getViewPoint(){ return ViewPoint; }
  Point getTarget(){ return Target; }
  Vector getViewDirection(){ return ViewDirection; }

  void setResolution(int,int);
  void  setViewPoint(Point v);
  void  setTarget(Point t);

  void LaunchRay(Ray *,float,float);
  void CreateCullingBox(Objet *);

  bool isGenerated();
  void isGenerated(bool);
};
}
#endif
