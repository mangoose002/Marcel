///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////


#ifndef _PLAN_H_
#define _PLAN_H_

#include "Core/Point.hh"
#include "Core/Ray.hh"
#include "Core/Vector.hh"
#include "Core/Color.hh"

#ifndef _OBJECT_HH_
#include "Core/Object.hh"
#endif

#include <sys/time.h>
#include "Core/Matrix.hh"

namespace Marcel {
	class Plan: public Objet
	{
	private:
		Color Carreau;
		double d; // cf ax*by+cz+d=0
	public:
		Vector N1;
		Vector N2;
	public:
		Point  O; // Point quelconque du plan
		Vector N; // Vecteur normal au plan en P
	public:

		Plan();
		Plan(Point, Vector, Color C, double D = 0, double Rfl = 0, double Rfr = 0, double = 0, int = 0, double = 0);
		Plan(Point, Vector, Color C, Color C2, double D = 0, double Rfl = 0, double Rfr = 0, double = 0, int = 0, double = 0);
		Plan(Point, Vector, char *, char *, double D = 0, double Rfl = 0, double Rfr = 0, double = 0, int = 0, double = 0);
		Plan(Point, Vector);
		Plan(const Plan& P){
			Carreau = P.Carreau;
			d       = P.d;
			N1      = P.N1;
			N2      = P.N2;
			O       = P.O;
			N       = P.N;
		}
		~Plan();
		double Distance(Point*);   // Distance d'un point a un plan
		Point Project(Point);
		Color getColor(Point*);
		Vector getNormal(Point *P, Point *UVW) { return N; }

		int Intersect(Tuple*, Droite*); // Intersection avec une droite
		void applyTransformation(Matrix *);
		void defineBoundingBox();
	};
}

#endif
