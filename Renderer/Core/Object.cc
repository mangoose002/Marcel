using namespace std;
///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////

#include "Core/Object.hh"

/*#include "Core/Vector.hh"
#include "Core/Ray.hh"
#include "Core/Droite.hh"
#include "Core/Point.hh"
#include "Core/Texture.hh"
#include "Core/CullingBox.hh"
*/

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

namespace Marcel{
    Objet::Objet() {
        Animate      = NULL;
        CullBox      = new CullingBox;
        BBox         = new BoundingBox;
        Mat          = NULL;

        InitialTransformation = new Matrix(4, 4);
        InitialTransformation->Identity();

        Colour         = Color(0, 0, 0);
        Reflexion      = Diffuse = Transparence = SurBrillance = 0;
        Refraction     = 1;
        HitProbability = 0;
    }

    Ray Objet::Reflect(Ray *R, Point *P, Vector *N) {
        Vector ReflectV = R->D - 2 * (R->D * (*N)) * (*N);
        return Ray((*P), ReflectV, R->Col * Colour, R->Level + 1);
    }
    //////////////////////////////////////////////////////////////////
    Ray Objet::Refract(Ray *R, Point *P, Vector *N) {
        double t, B, n;

        if (R->D * (*N) > 0)
            n = Refraction;
        else
            n = 1.0 / Refraction;

        B = (*N) * R->D;
        if (B < 0.0)
            B *= -1.0;

        t = 1.0 + n * n * (B * B - 1.0);
        if (t < 0.0)
            return Reflect(R, P, N);
        else
        {
            t = n * B - sqrt(t);
            Vector RefractV = n * R->D + t * (*N);
            return Ray((*P), RefractV, R->Col * Colour, R->Level + 1);
        }
    }

    void Objet::setColor(Color C)           { Colour = C;           }
    void Objet::setReflect(double R)        { Reflexion = R;        }
    void Objet::setRefract(double R)        { Refraction = R;       }
    void Objet::setDiffuse(double D)        { Diffuse = D;          }
    void Objet::setTrans(double T)          { Transparence = T;     }
    void Objet::setSurB(int S)              { SurBrillance = S;     }
    void Objet::setName(string s)           { Name = s;}

    float Objet::getDiffuse()       {return Diffuse;}
    float Objet::getReflection()    {return Reflexion;}
    float Objet::getRefraction()    {return Refraction;}

    float Objet::getTransparency() {
        if (Mat != NULL)
            return Mat->getTransparency();
        else
            return Transparence;
    }
    float Objet::getSurB()          { return SurBrillance;}
    string Objet::getName()         { return Name; }


    void Objet::setMaterial(Material *m)                           { Mat = m;           }
    Material *Objet::getMaterial(void)                             { return Mat;        }

    void Objet::setAnimation(Animation *a)                         { Animate = a;       }
    void Objet::addTransformation(Matrix m)                        { (*InitialTransformation) = (*InitialTransformation) * m; }

    void Objet::applyFrameAnimation(int i){
        if (Animate == NULL || i > Animate->getFrameNumber())
            return;

        applyTransformation(Animate->getTransformation(i));
    }
    ///////////////////////////////////////////////////////////////
    bool Objet::TestCullingBox(Droite *R) { 
        if(R->Level > 0)
            return true;
        
        return CullBox->Test((Ray *)R);  
    }
    void Objet::setCullingBoxValues(int x, int X, int y, int Y) {
        CullBox->setXValues(x, X);
        CullBox->setYValues(y, Y);
    }

    //void Objet::CreateCullingBox(Camera *camera){
    //}

    bool   Objet::isPolygon()         { return false;          }
    // this will be extended to other objects.

    CullingBox* Objet::getCullingBox() { return CullBox; }
    Vector      Objet::getNormal(Point *P, Point *UVW){ return No;   }
    Color       Objet::getColor(Point *P){ return Colour; }
}