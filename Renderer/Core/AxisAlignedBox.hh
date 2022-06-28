#ifndef _AXIS_ALIGNED_BOX_HH_
#define _AXIS_ALIGNED_BOX_HH_

#include <assert.h>
#include "Core/Point.hh"

namespace Marcel {
class AxisAlignedBox {
private:
	Point mMinimum;
	Point mMaximum;
	Point Center;
public:
	AxisAlignedBox(Point, Point);
	AxisAlignedBox(double, double, double, double, double, double);
	AxisAlignedBox (const AxisAlignedBox & rkBox);
	~AxisAlignedBox();

	inline const Point & getMinimum(void) const {
		return mMinimum;
	}

	inline Point & getMinimum(void) {
		return mMinimum;
	}

	inline const Point & getMaximum(void) const {
		return mMaximum;
	}

	inline Point & getMaximum(void) {
		return mMaximum;
	}
	inline void setExtents( const Point & min, const Point & max ) {
		assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
		        "The minimum corner of the box must be less than or equal to maximum corner" );

		mMinimum = min;
		mMaximum = max;
	}
	inline void setExtents(double mx, double my, double mz, double Mx, double My, double Mz ) {
		assert( (mx <= Mx && my <= My && mz <= Mz) &&
		        "The minimum corner of the box must be less than or equal to maximum corner" );

		mMinimum.x = mx;
		mMinimum.y = my;
		mMinimum.z = mz;

		mMaximum.x = Mx;
		mMaximum.y = My;
		mMaximum.z = Mz;
	}
	inline Point GetHalfSize(void) const{
		return (mMaximum - mMinimum) * 0.5;
	}
	inline Point GetCenter() const{
		return Center;
	}
};
}

#endif