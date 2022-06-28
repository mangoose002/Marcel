///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 07/01/1999                                 //
///////////////////////////////////////////////////////////////////////

#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

namespace Marcel{
	class Color
	{
	protected:
		float R;
		float G;
		float B;
	public:
		Color();
		Color(float Rc, float Gc, float Bc);
		~Color();

		inline float getRed()   {return R;};
		inline float getGreen() {return G;};
		inline float getBlue()  {return B;};
		inline void  setColor(float r, float g, float b) { R = r; G = g; B = b; };

		float Max();
		float MaxDifference(Color);

		void multiplyDouble(double);
		void add(Color);
		void add(Color*);
		Color multiply(Color *);

		friend Color    operator*(Color, Color);
		friend Color    operator*(float, Color);
		friend Color    operator*(Color, float);
		friend Color    operator+(Color, Color);
		friend Color    operator/(Color, int);
		friend std::ostream& operator<<(std::ostream&, Color);
	};
}

#endif
