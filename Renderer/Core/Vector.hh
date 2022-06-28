///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#include "Core/Point.hh"
#include "Core/RawVec3.hh"
#include <math.h>
#include <iostream>


namespace Marcel{
class Vector: public RawVec3{
public:
	Vector();
	Vector(double, double, double);
	Vector(Point, Point);
	Vector(Vector&);
	//Vector(RawVec3&);

	inline void   Normalize(){
		double N = Norme();
		x /= N;
		y /= N;
		z /= N;
	};

	inline double Norme()	{
		return sqrt(x * x + y * y + z * z);
	};

	Vector (const RawVec3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void   divide(double n);
	~Vector();
};

double   operator*(Vector, Vector); // Produit scalaire
Vector   operator/(Vector, Vector); // Produit Vectoriel

//Vector   operator*(Vector, double); // Produit par un scalaire
//Vector   operator*(double, Vector);

//Vector   operator+(Vector, Vector); // Addition
//Vector   operator-(Vector, Vector); // Soustraction

//                                              --> -->
//Vector   operator+(Point, Point);  // Calcul de OP1+OP2
//                                             --> --> --->
//Vector   operator-(Point, Point);  //Calcul de OP1-OP2=P2P1

//Point    operator+(Point, Vector); //Ajout d'un point et d'un vecteur
//Point    operator-(Point, Vector); //Ajout d'un point et d'un vecteur

//ostream& operator<<(ostream& , Vector);
//ostream& operator<<(ostream& , Vector*);
}
#endif
