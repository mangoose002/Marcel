///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#ifndef _RAY_H_
#define _RAY_H_

#include "Core/Vector.hh"
#include "Core/Point.hh"
#include "Core/Color.hh"
#include "Core/Droite.hh"

namespace Marcel{
	class Ray: public Droite
	{
	public:
	    Color Col;    // Indique la couleur du rayon en RGB

	    Ray(Point P, Vector L, Color C, int Le);
	    Ray(Point, Point);
	    Ray(Point, Vector, int L = 0);
	    Ray(Point*, Vector, int L = 0);
	    Ray(const Ray&);
	    void setColor(float, float, float);

	    Ray();
	    ~Ray();
	};

	ostream& operator<<(std::ostream&, Ray);
}
#endif
