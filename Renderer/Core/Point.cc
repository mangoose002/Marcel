using namespace std;
#include "Core/Point.hh"

#include <iostream>
#include <math.h>

namespace Marcel {
	Point::Point(double X, double Y, double Z): RawVec3(X,Y,Z) {}
	Point::Point() {}
	Point::~Point() {};


	double Point::Distance(Point P) {
		double N = (P.x - x) * (P.x - x) + (P.y - y) * (P.y - y) + (P.z - z) * (P.z - z);
		return (double)sqrt(N);
	}

	double Point::Distance(Point *P) {
		double N = (P->x - x) * (P->x - x) + (P->y - y) * (P->y - y) + (P->z - z) * (P->z - z);
		return (double)sqrt(N);
	}

	double Point::SquareDistance(Point *P) {
		return (P->x - x) * (P->x - x) + (P->y - y) * (P->y - y) + (P->z - z) * (P->z - z);
	}

	Point Point::Middle(Point P){
		return Point((P.x+x)/2,(P.y+y)/2,(P.z+z)/2);
	}
	Point Point::Middle(Point *P){
		return Point((P->x+x)/2,(P->y+y)/2,(P->z+z)/2);
	}

	bool Point::Between(Point *P1, Point *P2) {
		double D3 = P2->SquareDistance(P1);
		if (SquareDistance(P1) <= D3)
		{
			if (SquareDistance(P2) <= D3)
				return true;
			else
				return false;
		}
		else
			return false;
	}
}




