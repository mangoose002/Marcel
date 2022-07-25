#ifndef _SPOT_HH_
#define _SPOT_HH_

#include "Point.hh"
#include "Vector.hh"
#include "Color.hh"
#include "Light.hh"

namespace Marcel{
  class Spot: public Light
  {
  private:
    double cos_angle_1;
    double cos_angle_2;
    Vector Direction;

    Point Target;
  public:
    Spot();
    Spot(Point,Color,Point,double);
    ~Spot();

    void setHotSpot(double);
    void setFallOff(double);
    void setTarget(Point);
    
    Color getIntensity(Point *);
    bool  CanIlluminate(Point *);
  };
}
#endif
