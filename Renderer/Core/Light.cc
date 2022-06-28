using namespace std;
#include "Light.hh"
#include "Point.hh"
#include "Vector.hh"
#include "Color.hh"

namespace Marcel{
	void   Light::setOrigin(Point p) { O = p; }
	void   Light::setColor(Color c)  { Intensity = c;               }
	Point  Light::getOrigin()        { return O;                    }
	double Light::getRed()           { return Intensity.getRed();   }
	double Light::getGreen()         { return Intensity.getGreen(); }
	double Light::getBlue()          { return Intensity.getBlue();  }
}