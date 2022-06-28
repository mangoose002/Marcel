///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#ifndef _DROITE_H_
#define _DROITE_H_

#include "Core/Point.hh"
#include "Core/Vector.hh"

#include <iostream>

namespace Marcel{
    class Droite 
    {
    public:
        Point  O;
        Vector D;

        /// Screen coordinates This is only used if Level = 0
        double   x;
        double   y;

        /// Computational level
        char  Level;

    // Constructeur et destructeur de la classe
        Droite();
        Droite(Point, Vector);
        Droite(Point, Point);
        ~Droite();
        Point Project(Point*);

        inline Point getPoint(double t) {
            return Point(O.x + t * D.x, O.y + t * D.y, O.z + t * D.z);
        };

        char  isValid(Point);

        inline void  setDirection(const Vector _D){
            D.x = _D.x; D.y = _D.y; D.z = _D.z;
        };

        inline void  setDirection(const Vector* _D){
            D.x = _D->x; D.y = _D->y; D.z = _D->z;
        };

        void  setOrigin(Point*);
    // Methode qui renvoie la distance entre deux droites
        float Distance(Droite);
    // Methode qui renvoie le point d'intersection avec une autre droite
        double Intersect(Droite);

        friend ostream& operator<<(ostream&, Droite);
    };
}
#endif
