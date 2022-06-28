using namespace std;
#include "Core/Point.hh"
#include "Core/Vector.hh"
#include "Core/Tuple.hh"
#include <iostream>

namespace Marcel{
	Tuple::Tuple() {
		t = -1;
		// Vx = Vy = Vz = -1;
		obj = NULL;
	}
	Tuple::~Tuple() {
	}

	Tuple::Tuple(Vector N, double t1): t(t1) {
		// Vx = N.x;
		// Vy = N.y;
		// Vz = N.z;
		normal = N;
		obj = NULL;
	}

	Tuple::Tuple(Vector *N, double t1): t(t1) {
		// Vx = N->x;
		// Vy = N->y;
		// Vz = N->z;
		//mat = NULL;
		normal = (*N);
		obj = NULL;
	}
}










