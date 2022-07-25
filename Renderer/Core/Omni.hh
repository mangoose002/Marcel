///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#ifndef _OMNI_HH_
#define _OMNI_HH_

#include "Core/Point.hh"
#include "Core/Ray.hh"
#include "Core/Color.hh"
#include "Core/Light.hh"
#include <iostream>
#include <sys/time.h>

namespace Marcel{
	class Omni: public Light
	{
	 public:
	  Omni();
	  Omni(Point,Color); // Creation de base d'une lumiere blanche
	  ~Omni();
	  Color getIntensity(Point *);
	  bool CanIlluminate(Point *);
	};
}
#endif
