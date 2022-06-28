using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Ray.hh"
#include "Point.hh"
#include "Vector.hh"
#include "Color.hh"
#include "Droite.hh"


using namespace std;
namespace Marcel{
    Ray::Ray() {
        Level = 0;
    }

    Ray::Ray(Point a, Point b) {
        Level = 0;
        O = a;
        D = b - a;
        D.Normalize();
    }

    Ray::Ray(Point P, Vector Di, Color C, int L) {
        Level = L;
        O = P;
        D = Di; D.Normalize();
        Col = C;
    }

    Ray::Ray(Point P, Vector Di, int L) {
        D.x = Di.x; D.y = Di.y; D.z = Di.z;
        D.Normalize();; O = P; Level = L; Col = Color(255, 255, 255);
    }

    Ray::Ray(Point *P, Vector Di, int L) {
        D.x = Di.x; D.y = Di.y; D.z = Di.z;
        D.Normalize();;
        O.x = P->x; O.y = P->y; O.z = P->z;
        Level = L; Col = Color(255, 255, 255);
    }

    Ray::Ray(const Ray& R){
        Level = R.Level;
        O     = R.O;
        D     = R.D;
        Col   = R.Col;
    }

    Ray::~Ray() {}

    void Ray::setColor(float r, float g, float b) {
        Col.setColor(r, g, b);
    }
    /////////////////////////////////////////////////////////////
    ostream& operator<<(ostream& o, Ray R) {
        o << R.D << endl;
        return o;
    }
}

