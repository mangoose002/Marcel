using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 07/26/2002                                 //
///////////////////////////////////////////////////////////////////////
#include "Plan.hh"
#include "Point.hh"
#include "Vector.hh"
#include "Droite.hh"
#include "Ray.hh"

#include "Object.hh"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include "Texture.hh"

// Constructeur et destructeur
namespace Marcel{
	Plan::Plan(): Objet()
	{
		bump = 0;

		O = Point(0, 0, 0);
		N = Vector(0, 1, 0);

		N.Normalize();
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);
		if (N.x != 0 || N.y != 0)
			N1 = Vector(N.y, -N.x, 0);
		else
			N1 = Vector(0, N.z, -N.y);

		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();

		x = -5000; // Definition de la boite englobante.
		X = 5000;
		y = -5000;
		Y = 5000;
		z = -5000;
		Z = 5000;
	}

	Plan::~Plan() {
	}

	Plan::Plan(Point P, Vector V, Color C, double Dif, double Rfl, double Rfr, double Tr, int Sur, double Bump): Objet()
	{
	// Caracteristiques physiques de l'objet
		Colour = C / 255;
		Reflexion = Rfl; Refraction = Rfr, Diffuse = Dif; Transparence = Tr; SurBrillance = Sur;
		mapping = 0;
		bump = (int)Bump;
		O = P;
		N = V;
		N.Normalize();
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);
		if (N.x != 0 || N.y != 0)
			N1 = Vector(N.y, -N.x, 0);
		else
			N1 = Vector(0, N.z, -N.y);
		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();

		x = -5000; // Definition de la boite englobante.
		X = 5000;
		y = -5000;
		Y = 5000;
		z = -5000;
		Z = 5000;
	}

	Plan::Plan(Point P, Vector V, char *MapFile, char *BumpFile, double Dif, double Rfl, double Rfr, double Tr, int Sur, double Bump): Objet()
	{
	// Caracteristiques physiques de l'objet

		Reflexion = Rfl; Refraction = Rfr, Diffuse = Dif; Transparence = Tr; SurBrillance = Sur;
		mapping = 2; bump = (int)Bump;
		O = P;
		N = V;
		N.Normalize();
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);

		if (N.x != 0 || N.y != 0)
			N1 = Vector(N.y, -N.x, 0);
		else
			N1 = Vector(0, N.z, -N.y);
		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();

		x = -5000; // Definition de la boite englobante.
		X = 5000;
		y = -5000;
		Y = 5000;
		z = -5000;
		Z = 5000;
	}

	Plan::Plan(Point P, Vector V, Color C, Color C2, double Dif, double Rfl, double Rfr, double Tr, int Sur, double Bump): Objet()
	{
	// Caracteristiques physiques de l'objet
		Colour = C / 255;
		Carreau = C2 / 255;
		mapping = 1; bump = (int)(Bump);
		Reflexion = Rfl; Refraction = Rfr, Diffuse = Dif; Transparence = Tr; SurBrillance = Sur;

		O = P;
		N = V;
		N.Normalize();
		if (N.x != 0 || N.y != 0)
			N1 = Vector(N.y, -N.x, 0);
		else
			N1 = Vector(0, N.z, -N.y);
		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);

		x = -5000; // Definition de la boite engloba      T = Dr->getPoint(t);
		X = 5000;
		y = -5000;
		Y = 5000;
		z = -5000;
		Z = 5000;
	}

	Plan::Plan(Point P, Vector V): Objet()
	{
		O = P;
		N = V;
		N.Normalize();
		bump = 0;
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);

		N1 = Vector(-N.z, 0, N.x);
		/*
		if(N.x!=0 || N.y!=0)
		  N1=Vector(N.y,-N.x,0);
		else
		  N1=Vector(0,N.z,-N.y);
		*/

		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();

		x = -5000; // Definition de la boite englobante.
		X = 5000;
		y = -5000;
		Y = 5000;
		z = -5000;
		Z = 5000;
	}


	// Projection d'un point sur un plan
	Point Plan::Project(Point P)
	{ return (Point)(P - ((P - O) * N) * N); }

	// Calcul de la distance entre un point et un plan
	double Plan::Distance(Point *P)
	{
		double x, y, z, n, n2;
		x = P->x - O.x;
		y = P->y - O.y;
		z = P->z - O.z;
		n = x * N.x + y + N.y + z * N.z;
		n2 = n * n;
		return sqrt(n2 * N.x * N.x + n2 * N.y * N.y + n2 * N.z * N.z);
	}

	// Intersection entre le plan et une droite
	int Plan::Intersect(Tuple* C, Droite *Dr)
	{

		double A = N.x * Dr->O.x + N.y * Dr->O.y + N.z * Dr->O.z + d;
		double B = N.x * Dr->D.x + N.y * Dr->D.y + N.z * Dr->D.z;
		if (A * B < -1e-8)
		{
			double t = -A / B;

			(*C).normal = N;
			(*C).t = t;
			return 1;
		}
		else
		{
			(*C).t = -1;
			return 0;
		}
	}

	Color Plan::getColor(Point *P)
	{
		double x, y, z;
		int kx, ky;

		Vector OP;
		OP.x = P->x - O.x;
		OP.y = P->y - O.y;
		OP.z = P->z - O.z;
		switch (mapping)
		{
		case 1:
			x = OP.x;
			y = OP.y;
			z = OP.z;

			if (fabs(x) < 1e-8 && fabs(y) > 1e-8)
				x = z;
			if (fabs(y) < 1e-8 && fabs(x) > 1e-8)
				y = z;

			x = x - 1000000;
			y = y - 1000000;

			kx = abs((int)x / 30) % 2;
			ky = abs((int)y / 30) % 2;

			if (kx == ky)
				return Carreau;
			else
				return Colour;
			break;

		default:
			return Colour;
			break;
		}
	}

	void Plan::applyTransformation(Matrix *M)
	{
		O = (*M) * O;
		N = (*M) * N;

		N.Normalize();
		d = -(N.x * O.x + N.y * O.y + O.z * N.z);
		if (N.x != 0 || N.y != 0)
			N1 = Vector(N.y, -N.x, 0);
		else
			N1 = Vector(0, N.z, -N.y);
		N1.Normalize();
		N2 = N1 / N;
		N2.Normalize();
	}
}
