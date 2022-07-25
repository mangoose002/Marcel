#pragma once
#ifndef _TUPLE_HH_
#define _TUPLE_HH_


#include "Core/Vector.hh"
#include "Core/Material.hh"
//#include "Core/Object.hh"

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

    Tuple();
    Tuple(Vector, double=-1);
    Tuple(Vector*, double=-1);
    Tuple(Tuple &);
    ~Tuple();
    
  };
}

#endif
