#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include "Core/Scene.hh"
#include "Core/Point.hh"
#include "Core/Vector.hh"
#include "Core/Droite.hh"
#include "Core/Object.hh"
#include "Core/Ray.hh"
#include "Core/RowDispatcher.hh"

#include "Core/Omni.hh"
#include "Core/Octree.hh"
#include "Core/QuadTree.hh"

#include <vector>
#include "Core/Tuple.hh"
#include "Core/Color.hh"
#include "Core/Image.hh"
#include "Core/Camera.hh"
#include "Core/Material.hh"
#include <math.h>

#define BLOCK_SIZE 8

namespace Marcel{
    class Raytracer
    {
    protected:
        int ResX;
        int ResY;

    private:
        Scene *scene;
        RowDispatcher *Dispatcher;
        double Cx, Cy, Cz;

        int halfResX;
        int halfResY;

        int    MAX_LEVEL;
        int    BlockSize;
        int    Seuil;

        float  CoeffX;
        float  CoeffY;
        Point  Cam;
        Point  Lookat;
        Point  Eye;
        Vector CX;
        Vector CY;
        Vector CZ;

        Color Raytrace(Ray*, Objet*);
        Color Min(Color*);

        //Color computeDiffuse(Point *P, const Vector *N, Objet* O, const Ray *Ra, Material *);
        Color computeDiffuse(Tuple *CP, const Ray *Ra);

    public:
        Raytracer(const Raytracer &);
        Raytracer& operator= (const Raytracer &);
        Raytracer(Scene *, RowDispatcher *);
        ~Raytracer();

        /// Will be called from the Scene constructor.
        void  setLevel(const int L);
        void  setObjectList(vector<Objet *>*);

        void  ComputeNonAliasedLine(int, int, int blocksize = 1);
        void  ComputeAliasedPixel(int, int, int invert = 0, int computepixel = 0);

        //Calculation mode
        void Standard();
        void Alias();
        void Progressive(char**, int);

    };
}
#endif
