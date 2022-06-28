using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////
#include "Vector.hh"
#include "Point.hh"
#include "Droite.hh"
#include "Plan.hh"

#include <iostream>

namespace Marcel{
    Droite::Droite() {}

    Droite::Droite(Point P, Vector V): O(P), D(V) {}

    Droite::Droite(Point P1, Point P2): O(P1){ 
        D = P2 - P1; 
        D.Normalize();
    }

    Droite::~Droite() {
    }

    Point Droite::Project(Point *P) {
        double ProdScal = D.x * (P->x - O.x) + D.y * (P->y - O.y) + D.z * (P->z - O.z);
        
        return Point(O.x + D.x * ProdScal, O.y + D.y * ProdScal, O.z + D.z * ProdScal);
    }

    float Droite::Distance(Droite D2){
    // Calcul du plan parallele aux deux droite et qui
    // passe par l'origine de la premiere
        Plan P = Plan(O, D / D2.D);
        return P.Distance(&D2.O);
    }

    double Droite::Intersect(Droite D2){
        float Di = Distance(D2);
        if (Di == 0) {
            Vector V = D / (D / (D2.D));
            Plan P = Plan(O, V);
            Tuple *C = NULL;;
            P.Intersect(C, &D2);
            if (C != NULL)
            {
                double t = C->t;
                delete C;
                return t;
            }
        }
        else
            return -1;
    }

    char Droite::isValid(Point P){
        float PS = (P - O) * D;
        if (PS > 0)
            return 1;
        else
            return 0;
    }

    ostream& operator<<(ostream& o, Droite Dr)
    {
        o << "Point:   " << Dr.O << endl;
        o << "Vecteur: " << Dr.D << endl;
        return o;
    }

    void Droite::setOrigin(Point *P){
        O.x = P->x; O.y = P->y; O.z = P->z;
    }
}



