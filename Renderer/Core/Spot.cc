using namespace std;
#include "Spot.hh"
#include "Light.hh"
#include "Point.hh"
#include "Color.hh"
#include "Vector.hh"
#include <math.h>

namespace Marcel{
  Spot::Spot(Point o,Color c,Point d,double a)
  {
    CastShadows = true;
    O = o;
    Intensity = c/255;
    Direction = d-o;
    Direction.Normalize();
    cos_angle_1 = cos((a/180)*M_PI);
    cos_angle_2 = cos(((a+15)/180)*M_PI);
  }

  Spot::Spot(){ CastShadows = true; }

  Spot::~Spot(){}

  void Spot::setFallOff(double f) { cos_angle_2 = cos((f/180)*M_PI);}
  void Spot::setTarget(Point p)   { Target = p; Direction = Target - O; Direction.Normalize(); }
  void Spot::setHotSpot(double f) { cos_angle_1 = cos((f/180)*M_PI); };

  Color Spot::getIntensity(Point *p){
    Vector v = Vector((*p).x-O.x,(*p).y-O.y,(*p).z-O.z);
    v.Normalize();

    double cos_angle = v*Direction;
    if(cos_angle>cos_angle_1)
      return Intensity;

    if(cos_angle>cos_angle_2 && cos_angle<cos_angle_1)
      return Intensity*((cos_angle-cos_angle_2)/(cos_angle_1-cos_angle_2)) ;
  }

  int Spot::CanIlluminate(Point *p){
    Vector v = Vector((*p).x-O.x,(*p).y-O.y,(*p).z-O.z);
    v.Normalize();
    if(v*Direction>cos_angle_2)
      return 1;
    else
      return 0;
  }
}