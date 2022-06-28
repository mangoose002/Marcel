#ifndef _TUPLE_H_
#define _TUPLE_H_


#include "Core/Vector.hh"
#include "Core/Material.hh"

namespace Marcel{
  class Objet;  //For further declaration

  struct Tuple{
    // double   Vx;
    // double   Vy;
    // double   Vz;
    double   t;
    Vector   normal;
    Point    intersection;
    Color    color;
    
    Objet    *obj;
    //Material *mat;

    Tuple();
    Tuple(Vector, double=-1);
    Tuple(Vector*, double=-1);
    ~Tuple();
    
  };
}

#endif
