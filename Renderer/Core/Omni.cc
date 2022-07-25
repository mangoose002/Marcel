using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Point.hh"
#include "Ray.hh"
#include "Omni.hh"
#include "Light.hh"
#include "Color.hh"
#include "Vector.hh"
#include <sys/time.h>

namespace Marcel{
	Omni::Omni(){ 
		CastShadows = true; 
	}

	Omni::Omni(Point P, Color I)
	{ 
	  CastShadows = false;
	  O=P;
	  Intensity=I/255;
	}

	Omni::~Omni(){}

	Color Omni::getIntensity(Point *p)	{
	  return Intensity;
	}

	bool Omni::CanIlluminate(Point *p)	{
	  return true;
	}

}





