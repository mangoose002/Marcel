using namespace std;
#include "Point.hh"
#include "Vector.hh"
#include "Droite.hh"
#include "Triangle.hh"
#include <stdlib.h>

namespace Marcel{
	#define MIN(a,b)  a>b?b:a
	#define MAX(a,b)  a>b?a:b

	Triangle::Triangle(Point a, Point b, Point c): Objet() {
		A = a;
		B = b;
		C = c;

		AB = Droite(A, B);
		AC = Droite(A, C);
		BC = Droite(B, C);

		abn = ((Vector)(A - B)).Norme();
		acn = ((Vector)(A - C)).Norme();
		bcn = ((Vector)(B - C)).Norme();

		N  = (B - A) / (C - A);
		N1 = (B - A) / (C - A);
		n  = N1.Norme() * N1.Norme();

		V1 = A - C;
		V2 = A - B;

		N.Normalize();
		P = Plan(A, N);

		x =  5000; X = -5000;
		y =  5000; Y = -5000;
		z =  5000; Z = -5000;
		/////////////
		x = MIN(x,a.x); X=MAX(X,a.x);
		x = MIN(x,b.x); X=MAX(X,b.x);
		x = MIN(x,c.x); X=MAX(X,c.x);

		y = MIN(y,a.y); Y=MAX(Y,a.y);
		y = MIN(y,b.y); Y=MAX(Y,b.y);
		y = MIN(y,c.y); Y=MAX(Y,c.y);

		z = MIN(z,a.z); Z=MAX(Z,a.z);
		z = MIN(z,b.z); Z=MAX(Z,b.z);
		z = MIN(z,c.z); Z=MAX(Z,c.z);

		Area2 = sqrt(n);

		abn /= Area2;
		acn /= Area2;
		bcn /= Area2;

		Center = Point((x + X) / 2, (y + Y) / 2, (z + Z) / 2);
		R2     = ((x - X) * (x - X) + (y - Y) * (y - Y) + (z - Z) * (z - Z)) / 4.0;
	}

	Triangle::~Triangle() {

	}

	int Triangle::Intersect(Tuple *CP, Droite *Dr) {
		Objet::IntersectionNumber++;

		double a = N.x * Dr->O.x + N.y * Dr->O.y + N.z * Dr->O.z - (N.x * A.x + N.y * A.y + A.z * N.z);
		double b = N.x * Dr->D.x + N.y * Dr->D.y + N.z * Dr->D.z;

		(*CP).obj = NULL;

		if (a * b < -1e-8)
		{
			double t1 = -a / b;
			double r, s, t;
			double px, py, pz;

			if (t1 > 1e-8)
			{
				px = (Dr->O).x + t1 * (Dr->D).x;
				py = (Dr->O).y + t1 * (Dr->D).y;
				pz = (Dr->O).z + t1 * (Dr->D).z;

				double apx, apy, apz;
				apx = A.x - px;
				apy = A.y - py;
				apz = A.z - pz;

				double x1, y1, z1;
				x1 = apy * V1.z - V1.y * apz;
				y1 = apz * V1.x - V1.z * apx;
				z1 = apx * V1.y - V1.x * apy;
				s = x1 * N1.x + y1 * N1.y + z1 * N1.z;
				if (s > n || s < -1e-8) {
					(*CP).t = -1;
					return 0;
				}

				x1 = V2.y * apz - apy * V2.z;
				y1 = V2.z * apx - apz * V2.x;
				z1 = V2.x * apy - apx * V2.y;
				t = x1 * N1.x + y1 * N1.y + z1 * N1.z;
				if (t > n || t < -1e-8) {
					(*CP).t = -1;
					return 0;
				}

				r = n - ( t + s );
				if (r > n || r < -1e-8) {
					(*CP).t = -1;
					return 0;
				}

				(*CP).t   = t1;
				(*CP).obj = this;

				Objet::SuccessfulIntersectionNumber++;

				return 1;
			}
			else
			{
				(*CP).t = -1;
				return 0;
			}
		}

		(*CP).t = -1;
		return 0;
	}

	Color Triangle::getColor(Point *P) {
		if (Mat != NULL) {
			if (Mat->getMap1() != NULL && Mat->getMap1()->isLoaded()) {
				double x = P->x * UVAx + P->y * UVBx + P->z * UVCx;
				double y = P->x * UVAy + P->y * UVBy + P->z * UVCy;

				return Mat->getMap1()->getColor(x, y);
			}
			return Mat->getDiffuseColor();
		} else {
			return Colour;
		}
	}

	Vector Triangle::getNormal(Point *p,Point *UVW) {

		Vector bump;
		Vector Normal = computeShadedNormal(UVW);

		if (Mat != NULL) {
			if (Mat->getBumpMap() != NULL && Mat->getBumpMap()->isLoaded()) {
				Vector Nx;
				Vector Ny;

				int    X = (int)(p->x * P.N1.x + p->y * P.N1.y + p->z * P.N1.z);
				int    Y = (int)(p->x * P.N2.x + p->y * P.N2.y + p->z * P.N2.z);

				double x_gradient = (Mat->getBumpMap()->getGray(X - 1, Y) - Mat->getBumpMap()->getGray(X + 1, Y));
				double y_gradient = (Mat->getBumpMap()->getGray(X, Y - 1) - Mat->getBumpMap()->getGray(X, Y + 1));

				if (Normal.x != 0 || Normal.y != 0)
					Nx = Vector(Normal.y, -Normal.x, 0);
				else
					Nx = Vector(0, Normal.z, -Normal.y);

				Nx.Normalize();
				Ny = Nx / Normal;
				Ny.Normalize();

				bump = x_gradient * Nx + y_gradient * Ny;
			}
		}

		return Normal + bump;
	}


	int Triangle::TestIntersection(Droite *Dr) {

		//return 1;

		double Xl = Dr->O.x - Center.x;
		double Yl = Dr->O.y - Center.y;
		double Zl = Dr->O.z - Center.z;

		double b = Xl * Dr->D.x + Yl * Dr->D.y + Zl * Dr->D.z;
		double BB = b * b - (Xl * Xl + Yl * Yl + Zl * Zl - R2);

		if (BB < 0)
			return 0;

		double r = sqrt(BB);
		double t1 = -b - r;
		double t2 = -b + r;

		if (t1 < 1e-8 && t2 < 1e-8)
			return 0;

		return 1; // debug

		double Yy = -(Dr->D.z * Dr->D.z) - (Dr->D.x * Dr->D.x);
		double Yx = Dr->D.y * Dr->D.x;
		double Yz = Dr->D.z * Dr->D.y;

		double Xmin = 1e8; double Ymin = 1e8;
		double Xmax = -1e8; double Ymax = -1e8;
		double x1, y1;

		double a1 = (A.z - Dr->O.z) * Dr->D.x;
		double a2 = (A.x - Dr->O.x) * Dr->D.z;
		double a3 = (A.x - Dr->O.x) * Yx;
		double a4 = (A.y - Dr->O.y) * Yy;
		double a5 = (A.z - Dr->O.z) * Yz;

		double b1 = (B.z - Dr->O.z) * Dr->D.x;
		double b2 = (B.x - Dr->O.x) * Dr->D.z;
		double b3 = (B.x - Dr->O.x) * Yx;
		double b4 = (B.y - Dr->O.y) * Yy;
		double b5 = (B.z - Dr->O.z) * Yz;

		x1 = a1 - a2; // A
		y1 = a3 + a4 + a5;
		if (x1 > Xmax) Xmax = x1;
		if (x1 < Xmin) Xmin = x1;
		if (y1 > Ymax) Ymax = y1;
		if (y1 < Ymin) Ymin = y1;

		if (Xmin * Xmax < -1e-8 && Ymin * Ymax < -1e-8 && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		x1 = b1 - b2; // B
		y1 = b3 + b4 + b5;
		if (x1 > Xmax) Xmax = x1;
		if (x1 < Xmin) Xmin = x1;
		if (y1 > Ymax) Ymax = y1;
		if (y1 < Ymin) Ymin = y1;

		if (Xmin * Xmax < -1e-8 && Ymin * Ymax < -1e-8 && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		double c1 = (C.z - Dr->O.z) * Dr->D.x;
		double c2 = (C.x - Dr->O.x) * Dr->D.z;
		double c3 = (C.x - Dr->O.x) * Yx;
		double c4 = (C.y - Dr->O.y) * Yy;
		double c5 = (C.z - Dr->O.z) * Yz;

		x1 = c1 - c2; // C
		y1 = c3 + c4 + c5;
		if (x1 > Xmax) Xmax = x1;
		if (x1 < Xmin) Xmin = x1;
		if (y1 > Ymax) Ymax = y1;
		if (y1 < Ymin) Ymin = y1;

		if (Xmin * Xmax < -1e-8 && Ymin * Ymax < -1e-8 && Xmin != 1e8 && Xmax != -1e8 && Ymin != 1e8 && Ymax != -1e8)
			return 1;

		return 0;
	}

	void Triangle::applyTransformation(Matrix *M) {
		A = (*M) * A;
		B = (*M) * B;
		C = (*M) * C;

		N  = (B - A) / (C - A);
		N1 = (B - A) / (C - A);
		n  = N1.Norme() * N1.Norme();

		V1 = A - C;
		V2 = A - B;

		N.Normalize();
		P = Plan(A, N);

		x = 5000; // Initialize bounding box.
		X = -5000;
		y = 5000;
		Y = -5000;
		z = 5000;
		Z = -5000;
		/////////////
		if (A.x < x)
			x = A.x;
		if (A.x > X)
			X = A.x;

		if (B.x < x)
			x = B.x;
		if (B.x > X)
			X = B.x;

		if (C.x < x)
			x = C.x;
		if (C.x > X)
			X = C.x;
		//////////////
		if (A.y < y)
			y = A.y;
		if (A.y > Y)
			Y = A.y;

		if (B.y < y)
			y = B.y;
		if (B.y > Y)
			Y = B.y;

		if (C.y < y)
			y = C.y;
		if (C.y > Y)
			Y = C.y;
		///////////////
		if (A.z < z)
			z = A.z;
		if (A.z > Z)
			Z = A.z;

		if (B.z < z)
			z = B.z;
		if (B.z > Z)
			Z = B.z;

		if (C.z < z)
			z = C.z;
		if (C.z > Z)
			Z = C.z;

		Center = Point((x + X) / 2, (y + Y) / 2, (z + Z) / 2);
		R2     = ((x - X) * (x - X) + (y - Y) * (y - Y) + (z - Z) * (z - Z)) / 4.0;
	}

	void  Triangle::setShadingNormal(int n, Vector v) {
		v.Normalize();
		switch (n)	{
		case 1:
			ShadingNormalA = v; break;
		case 2:
			ShadingNormalB = v; break;
		case 3:
			ShadingNormalC = v; break;
		}
	}

	void   Triangle::setUVCoordinates(int n, double x, double y) {
		switch (n)	{
		case 1:
			UVAx = x;
			UVAy = y;
			break;
		case 2:
			UVBx = x;
			UVBy = y;
			break;
		case 3:
			UVCx = x;
			UVCy = y;
			break;
		}
	}

	Vector Triangle::computeShadedNormal(Point *P) {
		return Vector(
		           P->x * ShadingNormalA.x + P->y * ShadingNormalB.x + P->z * ShadingNormalC.x,
		           P->x * ShadingNormalA.y + P->y * ShadingNormalB.y + P->z * ShadingNormalC.y,
		           P->x * ShadingNormalA.z + P->y * ShadingNormalB.z + P->z * ShadingNormalC.z
		       );
	}

	Point Triangle::computeUVW(Point *P){
		Point P1 = BC.Project(P);
		Point P2 = AC.Project(P);
		Point P3 = AB.Project(P);

		return Point(
				(P->Distance(&P1) * bcn),
				(P->Distance(&P2) * acn),
				(P->Distance(&P3) * abn)
			);
	}
}
