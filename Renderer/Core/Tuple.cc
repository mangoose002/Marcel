using namespace std;
#include "Core/Point.hh"
#include "Core/Vector.hh"
#include "Core/Tuple.hh"
#include <iostream>

namespace Marcel{
	Tuple::Tuple() {
		t = -1;
		obj = NULL;
	}
	Tuple::~Tuple() {
	}

	Tuple::Tuple(Vector N, double t1): t(t1) {
		normal = N;
		obj = NULL;
	}

	Tuple::Tuple(Vector *N, double t1): t(t1) {
		normal = (*N);
		obj = NULL;
	}

	Tuple::Tuple(Tuple &LocalTuple){
		t = LocalTuple.t;
		normal = LocalTuple.normal;
		obj = LocalTuple.obj;
	}
}










