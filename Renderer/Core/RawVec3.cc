#include "Core/RawVec3.hh"
#include <iostream>

namespace Marcel {
	RawVec3::RawVec3(){
	}
	RawVec3::RawVec3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {
	}
	RawVec3::~RawVec3(){
	}
	RawVec3::RawVec3(const RawVec3& V) {
		x = V.x;
		y = V.y;
		z = V.z;
	}
	RawVec3 operator*(RawVec3 v,double d){
		return RawVec3(v.x*d,v.y*d,v.z*d);
	}
	RawVec3 operator*(double d,RawVec3 v){
		return RawVec3(v.x*d,v.y*d,v.z*d);
	}
	RawVec3 operator/(RawVec3 v,double d){
		return RawVec3(v.x/d,v.y/d,v.z/d);
	}
	RawVec3 operator/(double d,RawVec3 v){
		return RawVec3(v.x/d,v.y/d,v.z/d);
	}
	RawVec3 operator+(RawVec3 a,RawVec3 b){
		return RawVec3(a.x+b.x,a.y+b.y,a.z+b.z);
	}
	RawVec3 operator-(RawVec3 a,RawVec3 b){
		return RawVec3(a.x-b.x,a.y-b.y,a.z-b.z);
	}
	std::ostream& operator<<(std::ostream& o, RawVec3 v) {
		o << v.x << "\t" << v.y << "\t" << v.z; return o;
	}
}