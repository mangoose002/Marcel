#define GLM_ENABLE_EXPERIMENTAL

#include <sys/time.h>
#include <iostream>
#include <glm/glm.hpp> // vec3 normalize cross
#include "glm/ext.hpp"

#include "Core/Vector.hh"
#include "Core/Matrix.hh"

using namespace std;
using namespace Marcel;

int main(void){
/*	glm::vec3 a = glm::vec3(1,2,3);
	glm::vec3 b = glm::vec3(4,5,6);

	Marcel::Vector a1 = Marcel::Vector(1,2,3);
	Marcel::Vector b1 = Marcel::Vector(4,5,6);
	
	cout << glm::to_string(a) << endl;
	cout << a1 << endl;

	cout << glm::length(a) << endl;
	cout << a1.Norme() << endl;

	cout << glm::dot(a,b) << endl;
	cout << (a1*b1) << endl;

	timeval *dm = new timeval;
  	timeval *fm = new timeval;

	cout << "Cross" << endl;
	gettimeofday(dm, NULL);
	for(int i=0; i<10000000; i++){
		glm::dot(a,b);
	}
	gettimeofday(fm, NULL);
	cout << "GLM Time: " << (fm->tv_sec * 1000000 + fm->tv_usec - dm->tv_sec * 1000000 - dm->tv_usec) / 1000000.0 << endl;

	gettimeofday(dm, NULL);
	for(int i=0; i<10000000; i++){
		a1*b1;
	}
	gettimeofday(fm, NULL);
	cout << "Vector Time: " << (fm->tv_sec * 1000000 + fm->tv_usec - dm->tv_sec * 1000000 - dm->tv_usec) / 1000000.0 << endl;
	
	cout << "Normalize" << endl;
	cout << glm::to_string(glm::normalize(a)) << endl;
	a1.Normalize();
	cout << a1 << endl;*/

	Matrix mat(4,4);
	mat.setValue(0,0,1); mat.setValue(0,1,2); mat.setValue(0,2,3); mat.setValue(0,3,4);
	mat.setValue(1,0,8); mat.setValue(1,1,7); mat.setValue(1,2,6); mat.setValue(1,3,5);
	mat.setValue(2,0,9); mat.setValue(2,1,10); mat.setValue(2,2,11); mat.setValue(2,3,12);
	mat.setValue(3,0,16); mat.setValue(3,1,15); mat.setValue(3,2,14); mat.setValue(3,3,13);
	cout << mat << endl;

	Vector v(1,2,3);

	cout << mat*v << endl;
}


