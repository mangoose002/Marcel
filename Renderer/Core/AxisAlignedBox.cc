#include "Core/AxisAlignedBox.hh"

namespace Marcel{
	AxisAlignedBox::AxisAlignedBox(Point min,Point max){
		mMinimum = min;
		mMaximum = max;
		Center   = mMinimum.Middle(mMaximum);
	}
	AxisAlignedBox::AxisAlignedBox(double xmin,double ymin,double zmin, double xmax, double ymax, double zmax){
		mMinimum = Point(xmin,ymin,zmin);
		mMaximum = Point(xmax,ymax,zmax);
		Center   = mMinimum.Middle(mMaximum);
	}
	AxisAlignedBox::AxisAlignedBox (const AxisAlignedBox &rkBox){
		mMinimum = rkBox.mMinimum;
		mMaximum = rkBox.mMaximum;
		Center   = mMinimum.Middle(mMaximum);
	}
}