///////////////////////////////////////////////////////////////////////
//                  Marcel le Raytracer Parallele                    //
///////////////////////////////////////////////////////////////////////
//         Eric Mugnier - Pierre Lena - Guillaume Bourelly           //
//                     Jean Francois Morcillo                        //
///////////////////////////////////////////////////////////////////////
// Derniere Modification: 02/01/1999                                 //
///////////////////////////////////////////////////////////////////////

#ifndef _OBJECT_HH_
#define _OBJECT_HH_

using namespace std;

// Inclusion des classes de base
#include "Core/Ray.hh"
#include "Core/Vector.hh"
#include "Core/Point.hh"
#include "Core/Color.hh"
#ifndef _TUPLE_HH_
#include "Core/Tuple.hh"
#endif
#include "Core/Texture.hh"

#include "Core/CullingBox.hh"
#include "Core/BoundingBox.hh"

#include "Core/Animation.hh"
#include "Core/Matrix.hh"
#include "Core/Material.hh"

namespace Marcel{
    class Objet
    {
    protected:
        int bump;           // Flags de Bump
        string              Name;

        Material           *Mat;

        Matrix             *InitialTransformation;
        Animation          *Animate;
        CullingBox         *CullBox;
        BoundingBox        *BBox;

        int mapping;
        Vector  No;

        Color Colour;       // Couleur de l'objet
        float Diffuse;      // Diffusion  dans les trois longueurs d'onde
        float Reflexion;    // Reflexion  dans les trois longueurs d'onde
        float Refraction;   // Refraction dans les trois longueurs d'onde
        float Transparence; // Transparence
        int   SurBrillance; // Coeff de surbrillance (>=1)

        float HitProbability;
    public:
        Objet();

        virtual int    Intersect(Tuple*, Droite*) = 0;
        virtual void   applyTransformation(Matrix *) = 0;
        virtual string isKindOf() { return "Object"; }
        virtual        ~Objet() {};
        virtual Color  getColor(Point*);
        virtual Vector getNormal(Point *,Point *);
        virtual Point  computeUVW(Point *){ return Point(0,0,0); }
        virtual void   defineBoundingBox() = 0;

        // Renvoi le rayon reflechi
        Ray Reflect(Ray*, Point*, Vector*);
        Ray Refract(Ray*, Point*, Vector*);

        void setReflect(double);
        void setRefract(double);
        void setTrans(double);
        void setDiffuse(double);
        void setSurB(int);
        void setColor(Color);
        void setName(string);

        float  getDiffuse();
        float  getReflection();
        float  getRefraction();
        float  getTransparency();
        float  getSurB();
        string getName();

        BoundingBox*   getBoundingBox();
        CullingBox*    getCullingBox();

        bool  TestCullingBox  (Droite *);

        void setMaterial(Material *m);
        Material *getMaterial(void);

        void setCullingBoxValues(int, int, int, int);
        //void CreateCullingBox(Camera *);

        void setAnimation(Animation *);
        void addTransformation(Matrix);
        void applyFrameAnimation(int i);

        virtual bool isPolygon();
    };
}
#endif
