using namespace std;
#include "Core/Camera.hh"
#include "Core/Point.hh"
#include "Core/Vector.hh"
#include "Core/Object.hh"
#include "Core/Ray.hh"
#include "Core/Polygon.hh"

#include <iostream>
#include <vector>

namespace Marcel{

	#define MIN(a,b)  a<b?a:b;
	#define MAX(a,b)  a>b?a:b;

	Camera::Camera() {
		_isGenerated = false;
	}
	Camera::Camera(Point v, Point t, double f, int x, int y)
	{
		ViewPoint     = v;
		Target        = t;
		focus         = f / ((double)x / (double)y);
		XResolution   = x;
		YResolution   = y;
		ViewDirection = (Target - ViewPoint); 
		ViewDirection.Normalize();

		X             = Vector(ViewDirection.y, -ViewDirection.x,0);
		Y             = X / ViewDirection;

		X.Normalize();
		Y.Normalize();

		ViewPlaneOrigin = ViewPoint - ViewDirection * focus;
		ViewPlane = Plan(ViewPlaneOrigin, ViewDirection);
		_isGenerated = false;
	}

	Camera::~Camera() {}

	void Camera::setResolution(int x, int y){
		XResolution = x; 
		YResolution = y;
	}

	void Camera::setTarget(Point t){
		Target = t;

		ViewDirection = (Target - ViewPoint); 
		ViewDirection.Normalize();

		X             = Vector(ViewDirection.y, -ViewDirection.x,0);
		Y             = X / ViewDirection;

		X.Normalize();
		Y.Normalize();

		ViewPlaneOrigin = ViewPoint - ViewDirection * focus;
		ViewPlane = Plan(ViewPlaneOrigin, ViewDirection);

	}

	void Camera::getViewPlaneCoordinatesFromPoint(Point P, int *x, int *y){
		Vector OP = P - ViewPlaneOrigin;
		double xi, yi;

		xi = OP * X;
		yi = OP * Y;

		if (fabs(xi) < 1e-8)
			xi = 0;
		if (fabs(yi) < 1e-8)
			yi = 0;

		(*x) = (int)((xi / 36.0 + .5) * XResolution);
		(*y) = (int)((yi / ((36.0*YResolution)/XResolution) + .5) * YResolution);
	}

	void Camera::getPointFromViewPlaneCoordinates(Point *p, float x, float y){
		double xi;
		double yi;

		xi = (((double)x) / XResolution - 0.5) * 36;
		yi = (((double)y) / YResolution - 0.5) * ((36.0*YResolution)/XResolution);

		(*p).x = ViewPlaneOrigin.x + xi * X.x + yi * Y.x;
		(*p).y = ViewPlaneOrigin.y + xi * X.y + yi * Y.y;
		(*p).z = ViewPlaneOrigin.z + xi * X.z + yi * Y.z;
	}

	void Camera::LaunchRay(Ray *R, float x, float y){
		Point  ViewPlanePoint;
		Vector RayDirection;

		getPointFromViewPlaneCoordinates(&ViewPlanePoint, x, y);
		RayDirection = ViewPoint - ViewPlanePoint;
		RayDirection.Normalize();

		R->setOrigin(&ViewPoint);
		R->setDirection(&RayDirection);
		R->Level = 0;
		R->x = x;
		R->y = y;
	}

	void Camera::CreateCullingBox(Objet *O){
		Point TablePoint[8];
		Ray R;
		Tuple c;

		TablePoint[0] = Point(O->x, O->y, O->z);
		TablePoint[1] = Point(O->x, O->y, O->Z);
		TablePoint[2] = Point(O->x, O->Y, O->z);
		TablePoint[3] = Point(O->x, O->Y, O->Z);

		TablePoint[4] = Point(O->X, O->y, O->z);
		TablePoint[5] = Point(O->X, O->y, O->Z);
		TablePoint[6] = Point(O->X, O->Y, O->z);
		TablePoint[7] = Point(O->X, O->Y, O->Z);

		int xmin, xmax;
		int ymin, ymax;
		int x, y;
		xmin = ymin =   5000;
		xmax = ymax =  -5000;


		for (int i = 0; i < 8; i++)	{
			R = Ray(TablePoint[i], ViewPoint);
			ViewPlane.Intersect(&c, &R);
			if (c.t > 1e-8)		{
				Point  p = R.getPoint(c.t);
				getViewPlaneCoordinatesFromPoint(p, &x, &y);

				xmax = MAX(x, xmax);
				xmin = MIN(x, xmin);
				ymax = MAX(y, ymax);
				ymin = MIN(y, ymin);
			}
		}

		O->setCullingBoxValues(xmin, xmax, ymin, ymax);

		if (O->isPolygon())	{
			vector<Objet *> *TriangleList = ((Polygon *)O)->getTriangleList();
			Objet *mObject;
			for (int yy = 0; yy < TriangleList->size(); yy++) {
				mObject = TriangleList->at(yy);
				if (mObject != NULL)
					CreateCullingBox(mObject);
			}
		}
	}

	bool Camera::isGenerated(){
		return _isGenerated;
	}

	void Camera::isGenerated(bool state){
		_isGenerated = state;
	}
}