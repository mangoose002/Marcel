///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#ifndef _POINT_H_
#define _POINT_H_

#include "Core/RawVec3.hh"
#include <iostream>




namespace Marcel{
  class Point : public RawVec3{
   public:
    Point();
    Point(double, double, double);
    Point (const RawVec3& v) {
      x = v.x;
      y = v.y;
      z = v.z;
    }

    double Distance(Point);
    double Distance(Point*);
    Point  Middle(Point);
    Point  Middle(Point *);

    bool   Between(Point*,Point*);
    double SquareDistance(Point*);
    ~Point();
  };
}
#endif
