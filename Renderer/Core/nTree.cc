using namespace std;
#include "Core/nTree.hh"
#include "Core/Object.hh"
#include "Core/Droite.hh"
#include "Core/Ray.hh"
#include "Core/Tuple.hh"
#include "Core/QuadTree.hh"
#include "Core/Octree.hh"
#include <iostream>
#include <vector>
#include <stdlib.h>

namespace Marcel {

#define EPSILON 1e-8

#define MIN(a,b)  a>b?b:a
#define MAX(a,b)  a>b?a:b

nTree::nTree(int n) {
    MAX_LEVEL  = 0;
    nbElement  = n;
    O          = new nTree*[n];
    ObjectList = new vector<Objet *>;
    LocalCount = 0;
    TotalCount = 0;

    for (int i = 0; i < nbElement; i++)
        O[i] = NULL;
}

nTree::~nTree() {
    delete O;
}

int nTree::Intersect(Droite *D, Tuple *C) {
    Tuple LocalTuple;
    int    foundobject = 0;

    if (TestIntersection(D) == 1) {
        for (int i = 0; i < nbElement; i++)
            if (O[i] != NULL) // To avoid empty leaves
                O[i]->Intersect(D, C);

        if (LocalCount > 0) // If there are object in the octree.
        {
            Objet *object;

            //for (int yy = 0; yy < ObjectList->size(); yy++) {
            for (vector<Objet *>::iterator it = ObjectList->begin(); it != ObjectList->end(); ++it) {

                object = *(it);

                if ( object != C->obj || (C->obj != NULL && C->obj->getTransparency() > 0) || (C->obj != NULL && C->obj->getReflection() > 0)) {
                    if (object->TestCullingBox(D)) {
                        object->Intersect(&LocalTuple, D);
                        if (LocalTuple.t > 1e-8 && LocalTuple.t < C->t) {
                            (*C).normal = LocalTuple.normal;
                            (*C).t      = LocalTuple.t;
                            (*C).obj    = LocalTuple.obj;
                            foundobject = 1;
                        }
                    }
                }
            }
        }
        return foundobject;
    }
    return 0;
}

int nTree::Add(Objet *o)
{
    int p;
    if (o == NULL)
        return 0;

    p = CheckObject(o);
    if (p == -1 || Level == MAX_LEVEL)    {
too_small:
        ObjectList->push_back(o);
        LocalCount++ ;
        if (F != NULL)
            F->TotalCount++;
        return 1;
    } else {
        if (nbElement == 4) {
            if (((QuadTree *)this)->getWidth() < 8 || ((QuadTree *)this)->getHeight() < 8)
                goto too_small;
        }

        // On peut rentrer dans un fils, s'il n'existe pas, on le cree et on ajoute l'objet
        if (O[p] == NULL)
            CreateElement(p);
        O[p]->Add(o);

    }
    return 1;
}
double nTree::getHeight() {
}
double nTree::getWidth() {
}
double nTree::getDepth() {
}
int    nTree::getCount() {
    return ObjectList->size();
}
int    nTree::getTotalCount()  {
    return TotalCount;
}
void   nTree::setMaxLevel(int l)           {
    MAX_LEVEL = l;
}
void   nTree::Draw(Image *, int, int, Color)  {
}
int    nTree::ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob) {
}

int nTree::getCumulativeCount()
{
    int Count = 0;
    for (int i = 0; i < nbElement; i++)
        if (O[i] != NULL)
            Count = Count + O[i]->getCumulativeCount();

    Count += getCount();
    return Count;
}

void nTree::UpdateCount(int *C)
{
    for (int i = 0; i < nbElement; i++)
        if (O[i] != NULL)
            O[i]->UpdateCount(C);

    if (Level > (*C))
        (*C)++;
    LocalCount = getCount();
    TotalCount = getCumulativeCount();
}

}