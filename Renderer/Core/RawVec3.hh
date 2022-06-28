#ifndef _RAW_VEC3_H_
#define _RAW_VEC3_H_

#include <iostream>

namespace Marcel{
	class Vector;
	class Point;

	struct RawVec3{
		public:
			double x;
			double y;
			double z;

			RawVec3();
			RawVec3(double,double,double);
			RawVec3(const RawVec3&);
			~RawVec3();
	};

	RawVec3 operator*(RawVec3,double);
	RawVec3 operator*(double,RawVec3);
	RawVec3 operator/(RawVec3,double);
	RawVec3 operator/(double,RawVec3);
	RawVec3 operator+(RawVec3,RawVec3);
	RawVec3 operator-(RawVec3,RawVec3);
	std::ostream& operator<<(std::ostream&, RawVec3);
	//Vector& operator= (const RawVec3& v) {return Vector(v->x,v->y,v->z);}
}
#endif