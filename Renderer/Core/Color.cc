using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////

#include "Color.hh"
#include <iostream>
#include <stdlib.h>

namespace Marcel {
	Color::Color() {R = G = B = 0;}
	Color::~Color() {}

	Color::Color(float Rc, float Gc, float Bc): R(Rc), G(Gc), B(Bc) {} // On stock des intensitees

	float Color::Max()	{
		if (R >= G && R >= B)
			return R;

		if (G >= R && G >= B)
			return G;

		if (B >= R && B >= G)
			return B;

		return 1;
	}

	float Color::MaxDifference(Color C)	{
		int RD = (int)(C.R - R); RD = abs(RD);
		int GD = (int)(C.G - G); GD = abs(GD);
		int BD = (int)(C.B - B); BD = abs(BD);

		if (RD >= GD && RD >= BD)
			return RD;

		if (GD >= RD && GD >= BD)
			return GD;

		if (BD >= RD && BD >= GD)
			return BD;

		return 1;
	}

	Color operator*(Color A, Color C)	{
		return Color(A.R * C.R, A.G * C.G, A.B * C.B);
	}

	Color operator*(Color A, float C)	{
		return Color(A.R * C, A.G * C, A.B * C);
	}

	Color operator*(float C, Color A)
	{ return Color(A.R * C, A.G * C, A.B * C); }

	Color operator+(Color A, Color C)
	{ return Color(A.R + C.R, A.G + C.G, A.B + C.B); }

	Color operator/(Color A, int C)
	{ return Color(A.R / C, A.G / C, A.B / C); }

	ostream& operator<<(ostream& o, Color C)
	{ o << C.R << "\t" << C.G << "\t" << C.B; return o; }

	void Color::multiplyDouble(double a)	{
		R = R * a;
		G = G * a;
		B = B * a;
	}

	void Color::add(Color C)	{
		R += C.R;
		G += C.G;
		B += C.B;
	}

	void Color::add(Color *C)	{
		R += C->R;
		G += C->G;
		B += C->B;
	}

	Color Color::multiply(Color *C1)	{
		return Color(C1->R * R, C1->G * G, C1->B * B);
	}
}