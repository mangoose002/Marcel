using namespace std;
#include "Core/nTree.hh"
#include "Core/Octree.hh"
#include "Core/Object.hh"

#include <iostream>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include "Droite.hh"
#include "Ray.hh"
#include "Tuple.hh"

namespace Marcel{

	#define EPSILON 1e-8
	#define MIN(a,b)   a>b?b:a
	#define MAX(a,b)   a>b?a:b

	Octree::Octree(double xm, double Xm, double ym, double Ym, double zm, double Zm): nTree(8)	{
		Level      = 0;
		Position   = 0;
		F          = NULL;
		xmin       = xm;		xmax       = Xm;
		ymin       = ym;		ymax       = Ym;
		zmin       = zm;		zmax       = Zm;

		BoundingMin = Point(xm,ym,zm);
		BoundingMax = Point(Xm,Ym,Zm);
		Center      = BoundingMin.Middle(&BoundingMax);
		R           = BoundingMax.SquareDistance(&BoundingMin) / 4;

		X = (xmin + xmax) / 2;
		Y = (ymin + ymax) / 2;
		Z = (zmin + zmax) / 2;
	}

	Octree::Octree(int level, int position, Octree* Father): nTree(8)	{
		MAX_LEVEL = Father->MAX_LEVEL;
		Level = level;
		Position = position;
		F = Father;

		if (Father == NULL){ // Creation du premier Octree
			xmin = ymin = zmin = -10000;
			xmax = ymax = zmax =  10000;
			BoundingMin = Point(-10000,-10000,-10000);
			BoundingMax = Point( 10000, 10000, 10000);
		} else {
			switch (position){
			case 0:
				xmin = Father->xmin;
				xmax = (Father->xmax + Father->xmin) / 2;
				ymin = Father->ymin;
				ymax = (Father->ymax + Father->ymin) / 2;
				zmin = Father->zmin;
				zmax = (Father->zmax + Father->zmin) / 2;
				break;
			case 1:
				xmin = (Father->xmin + Father->xmax) / 2;
				xmax = Father->xmax;
				ymin = Father->ymin;
				ymax = (Father->ymax + Father->ymin) / 2;
				zmin = Father->zmin;
				zmax = (Father->zmax + Father->zmin) / 2;
				break;
			case 2:
				xmin = Father->xmin;
				xmax = (Father->xmax + Father->xmin) / 2;
				ymin = (Father->ymax + Father->ymin) / 2;
				ymax = Father->ymax;
				zmin = Father->zmin;
				zmax = (Father->zmax + Father->zmin) / 2;
				break;
			case 3:
				xmin = (Father->xmin + Father->xmax) / 2;
				xmax = Father->xmax;
				ymin = (Father->ymax + Father->ymin) / 2;
				ymax = Father->ymax;
				zmin = Father->zmin;
				zmax = (Father->zmax + Father->zmin) / 2;
				break;
			case 4:
				xmin = Father->xmin;
				xmax = (Father->xmax + Father->xmin) / 2;
				ymin = Father->ymin;
				ymax = (Father->ymax + Father->ymin) / 2;
				zmin = (Father->zmax + Father->zmin) / 2;
				zmax = Father->zmax;
				break;
			case 5:
				xmin = (Father->xmin + Father->xmax) / 2;
				xmax = Father->xmax;
				ymin = Father->ymin;
				ymax = (Father->ymax + Father->ymin) / 2;
				zmin = (Father->zmax + Father->zmin) / 2;
				zmax = Father->zmax;
				break;
			case 6:
				xmin = Father->xmin;
				xmax = (Father->xmax + Father->xmin) / 2;
				ymin = (Father->ymax + Father->ymin) / 2;
				ymax = Father->ymax;
				zmin = (Father->zmax + Father->zmin) / 2;
				zmax = Father->zmax;
				break;
			case 7:
				xmin = (Father->xmin + Father->xmax) / 2;
				xmax = Father->xmax;
				ymin = (Father->ymax + Father->ymin) / 2;
				ymax = Father->ymax;
				zmin = (Father->zmax + Father->zmin) / 2;
				zmax = Father->zmax;
				break;
			}
		}

		Center      = BoundingMin.Middle(&BoundingMax);
		R           = BoundingMax.SquareDistance(&BoundingMin) / 4;

		R = ((xmin - xmax) * (xmin - xmax) + (ymin - ymax) * (ymin - ymax) + (zmin - zmax) * (zmin - zmax)) / 4.0;
		X = (xmin + xmax) / 2;
		Y = (ymin + ymax) / 2;
		Z = (zmin + zmax) / 2;
	}

	void Octree::CreateElement(int i) {
		O[i] = new Octree(Level + 1, i, this);
	}

	Octree::~Octree()
	{
		if (ObjectList != NULL)
			delete ObjectList;

		for (int i = 0; i < nbElement; i++)
			if (O[i] != NULL)
				delete O[i];
	}

	int Octree::ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob)
	{
		int found = 0;
		if (TestIntersection(D) == true){
			if (LocalCount > 0){
				Tuple LocalTuple;
				Objet *object;

				for(vector<Objet *>::iterator it = ObjectList->begin(); it != ObjectList->end(); ++it) {
					object = (*it);

					if (object != Ob){
						object->Intersect(&LocalTuple, D);
						if (LocalTuple.t > EPSILON && LocalTuple.t < t){
							if (LocalTuple.obj->getMaterial() != NULL)
								if (LocalTuple.obj->getMaterial()->getTransparency() == 0) {
									(*NL) = (*NS) = 0;
									return -1;
								}

							found = 1;

							if (LocalTuple.obj->getMaterial() != NULL) {
								(*NL) = (*NL) * LocalTuple.obj->getMaterial()->getTransparency();
								(*NS) = (*NS) * LocalTuple.obj->getMaterial()->getTransparency();
							}
						}
					}
				}
			}

			for (int i = 0; i < nbElement; i++)
				if (O[i] != NULL) // To avoid empty leaves
					if (O[i]->ComputeLight(D, t, NL, NS, Ob) == -1)
						return -1;
		}

		return found;
	}

	bool Octree::TestIntersection(Droite *Dr){
		// double tx0 = (xmin - Dr->O.x) / Dr->D.x;
		// double tx1 = (xmax - Dr->O.x) / Dr->D.x;
		// double ty0 = (ymin - Dr->O.y) / Dr->D.y;
		// double ty1 = (ymax - Dr->O.y) / Dr->D.y;
		// double tz0 = (zmin - Dr->O.z) / Dr->D.z;
		// double tz1 = (zmax - Dr->O.z) / Dr->D.z;

		// double tmin = MAX(MAX(tx0,ty0),tz0);
		// double tmax = MIN(MIN(tx1,ty1),tz1);

		// if(tmin < tmax && tmax>0)
		// 	return true;

		// return false;
		
		// /////////////////////
		double Xl = Dr->O.x - X;
		double Yl = Dr->O.y - Y;
		double Zl = Dr->O.z - Z;

		double b = Xl * Dr->D.x + Yl * Dr->D.y + Zl * Dr->D.z;
		double BB = b * b - (Xl * Xl + Yl * Yl + Zl * Zl - R);

		if (BB < 0)
			return 0;

		double r = sqrt(BB);
		double t1 = -b - r;
		double t2 = -b + r;

		if (t1 < EPSILON && t2 < EPSILON)
			return 0;

		double Yy = -(Dr->D.z * Dr->D.z) - (Dr->D.x * Dr->D.x);
		double Yx = Dr->D.y * Dr->D.x;
		double Yz = Dr->D.z * Dr->D.y;

		//On determine le point qui a le x le plus faible et celui qui a le x le +
		//grand, on fait de meme pour y
		double Xmin =  1e8; double Ymin =  1e8;
		double Xmax = -1e8; double Ymax = -1e8;
		double x, y;

		double a1 = (zmin - Dr->O.z) * Dr->D.x;
		double a2 = (xmin - Dr->O.x) * Dr->D.z;
		double a3 = (xmin - Dr->O.x) * Yx;
		double a4 = (ymin - Dr->O.y) * Yy;
		double a5 = (zmin - Dr->O.z) * Yz;

		double h1 = (zmax - Dr->O.z) * Dr->D.x;
		double h2 = (xmax - Dr->O.x) * Dr->D.z;
		double h3 = (xmax - Dr->O.x) * Yx;
		double h4 = (ymax - Dr->O.y) * Yy;
		double h5 = (zmax - Dr->O.z) * Yz;

		x = a1 - a2; // A
		y = a3 + a4 + a5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = h1 - h2; // H
		y = h3 + h4 + h5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = h1 - a2; // D
		y = a3 + a4 + h5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = a1 - h2; // B
		y = h3 + a4 + a5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = h1 - h2; // C
		y = h3 + a4 + h5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = h1 - a2; // E
		y = a3 + h4 + h5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = a1 - a2; // F
		y = a3 + h4 + a5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x = h1 - h2; // G
		y = h3 + h4 + a5;
		Xmax = MAX(Xmax,x);
		Xmin = MIN(Xmin,x);
		Ymax = MAX(Ymax,y);
		Ymin = MIN(Ymin,y);
		if (Xmin * Xmax < -EPSILON && Ymin * Ymax < -EPSILON && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		return 0;
	}

	int Octree::CheckObject(Objet *o){
		double Xmin = xmin;
		double Xmax = (xmax + xmin) / 2;
		double Ymin = ymin;
		double Ymax = (ymax + ymin) / 2;
		double Zmin = zmin;
		double Zmax = (zmax + zmin) / 2;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 0;

		Xmin = (xmin + xmax) / 2;
		Xmax = xmax;
		Ymin = ymin;
		Ymax = (ymax + ymin) / 2;
		Zmin = zmin;
		Zmax = (zmax + zmin) / 2;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 1;

		Xmin = xmin;
		Xmax = (xmax + xmin) / 2;
		Ymin = (ymax + ymin) / 2;
		Ymax = ymax;
		Zmin = zmin;
		Zmax = (zmax + zmin) / 2;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 2;

		Xmin = (xmin + xmax) / 2;
		Xmax = xmax;
		Ymin = (ymax + ymin) / 2;
		Ymax = ymax;
		Zmin = zmin;
		Zmax = (zmax + zmin) / 2;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 3;

		Xmin = xmin;
		Xmax = (xmax + xmin) / 2;
		Ymin = ymin;
		Ymax = (ymax + ymin) / 2;
		Zmin = (zmax + zmin) / 2;
		Zmax = zmax;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 4;

		Xmin = (xmin + xmax) / 2;
		Xmax = xmax;
		Ymin = ymin;
		Ymax = (ymax + ymin) / 2;
		Zmin = (zmax + zmin) / 2;
		Zmax = zmax;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 5;

		Xmin = xmin;
		Xmax = (xmax + xmin) / 2;
		Ymin = (ymax + ymin) / 2;
		Ymax = ymax;
		Zmin = (zmax + zmin) / 2;
		Zmax = zmax;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 6;

		Xmin = (xmin + xmax) / 2;
		Xmax = xmax;
		Ymin = (ymax + ymin) / 2;
		Ymax = ymax;
		Zmin = (zmax + zmin) / 2;
		Zmax = zmax;
		if (o->x >= Xmin && o->X <= Xmax && o->y >= Ymin && o->Y <= Ymax && o->z >= Zmin && o->Z <= Zmax)
			return 7;

		return -1;
	}

	double Octree::getWidth()  { return fabs(xmax - xmin); }
	double Octree::getHeight() { return fabs(ymax - ymin); }
	double Octree::getDepth()  { return fabs(zmax - zmin); }

	Point  Octree::getMinBound(){
		return Point(xmin,ymin,zmin);
	}
	Point  Octree::getMaxBound(){
		return Point(xmax,ymax,zmax);
	}
}