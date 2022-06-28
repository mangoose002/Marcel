#define GLM_ENABLE_EXPERIMENTAL

#include <sys/time.h>
#include <iostream>
#include <glm/glm.hpp> // vec3 normalize cross
#include "glm/ext.hpp"

#include "Renderer/Core/Vector.hh"
//#include <glm/gtx/fast_square_root.hpp> // fastNormalize

glm::vec3 computeNormal(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c)
{
    return glm::normalize(glm::cross(c - a, b - a));
}

glm::vec3 computeFastNormal(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c)
{
    return glm::fastNormalize(glm::cross(c - a, b - a));
}

int main(void){
	timeval *dm, *fm;

	glm::vec3 a =glm::vec3(1,1,1);
	glm::vec3 b =glm::vec3(1,2,1);
	glm::vec3 c =glm::vec3(1,1,2);

	Marcel::Vector a1 = Marcel::Vector(1,1,1);
	Marcel::Vector b1 = Marcel::Vector(1,2,1);
	Marcel::Vector c1 = Marcel::Vector(1,1,2);

  	dm=new timeval;
	fm=new timeval;

	gettimeofday(dm,NULL);
	for(uint64_t i=0; i<100000000; i++){
		computeNormal(a,b,c);
	}
	gettimeofday(fm,NULL);
	std::cerr << "Stayed: " << (fm->tv_sec*1000000+fm->tv_usec -dm->tv_sec*1000000-dm->tv_usec)/1000000.0 << std::endl;
	
	gettimeofday(dm,NULL);
	for(uint64_t i=0; i<100000000; i++){
		((c1 - a1)/(b1 - a1)); //->Normalize();
	}
	gettimeofday(fm,NULL);
	std::cerr << "Stayed: " << (fm->tv_sec*1000000+fm->tv_usec -dm->tv_sec*1000000-dm->tv_usec)/1000000.0 << std::endl;
}


