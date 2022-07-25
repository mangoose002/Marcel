// Last Modified: 07/26/2002

#ifndef _LIGHT_HH_
#define _LIGHT_HH_

#include "Point.hh"
#include "Ray.hh"
#include "Color.hh"
#include <iostream>
#include <sys/time.h>

namespace Marcel{
  class Light
  {
   protected:
    Color Intensity;
    Point O;
   public:
    void setColor(Color);
    void setOrigin(Point);

    virtual ~Light(){};
    Point  getOrigin();
    virtual Color getIntensity(Point *)=0;
    virtual bool CanIlluminate(Point *)=0;
    double getBlue();
    double getGreen();
    double getRed();

    bool   CastShadows;
  };
  }
#endif
