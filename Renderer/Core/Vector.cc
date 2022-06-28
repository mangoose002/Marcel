using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#include "Vector.hh"
#include "Point.hh"

#include <iostream>
#include <math.h>

namespace Marcel{
	Vector::Vector() {
		x = y = z = 0;
	}

	Vector::Vector(double X, double Y, double Z): RawVec3(X,Y,Z) {}

	Vector::Vector(Point A, Point B) {  // Construction du vecteur AB
		x = B.x - A.x;
		y = B.y - A.y;
		z = B.z - A.z;
	}

	Vector::~Vector() {
	};
	Vector::Vector(Vector &v){
		x = v.x;
		y = v.y;
		z = v.z;
	}
/*	Vector::Vector(RawVec3 &v){
		x = v.x;
		y = v.y;
		z = v.z;
	}*/

	void Vector::divide(double N) {
		x /= N;
		y /= N;
		z /= N;
	}

	double operator*(Vector V1, Vector V2) { //Produit Scalaire
		return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
	}

	Vector operator/(Vector V1, Vector V2) { // Produit Vectoriel
		double X, Y, Z;

		X = V1.y * V2.z - V1.z * V2.y;
		Y = V1.z * V2.x - V1.x * V2.z;
		Z = V1.x * V2.y - V1.y * V2.x;

		return Vector(X, Y, Z);
	}
}