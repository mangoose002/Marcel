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
	public:
		float R;
		float G;
		float B;
		Color();
		Color(float Rc, float Gc, float Bc);
		~Color();

		inline float getRed()   {return R;};
		inline float getGreen() {return G;};
		inline float getBlue()  {return B;};
		inline void  setColor(float r, float g, float b) { R = r; G = g; B = b; };
		inline Color(const Color &C){
			R = C.R;
			G = C.G;
			B = C.B;
		}

		float Max();
		float MaxDifference(Color);

		void multiplyDouble(double);
		void add(Color);
		void add(Color*);
		Color multiply(Color *);

		friend Color    operator*(const Color&, const Color&);
		friend Color    operator*(float, const Color&);
		friend Color    operator*(const Color&, float);
		friend Color    operator+(const Color&, const Color&);
		friend Color    operator/(const Color&, int);
		friend std::ostream& operator<<(std::ostream&, Color);
	};
}

#endif
