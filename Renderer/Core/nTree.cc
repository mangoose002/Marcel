using namespace std;
#include "Core/nTree.hh"
#include "Core/QuadTree.hh"
#include "Core/Triangle.hh"
#include <iostream>
#include <vector>
#include <stdlib.h>

namespace Marcel {

#define EPSILON 1e-8

#define MIN(a,b)  a>b?b:a
#define MAX(a,b)  a>b?a:b

nTree::nTree(int n) {
    Level      = 0;
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

    if (TestIntersection(D) == true) {
        for (int i = 0; i < nbElement; i++){
            if (O[i] != NULL){ // To avoid empty leaves{
                O[i]->Intersect(D, C);
            }
        }
        for(Objet*& object: *ObjectList){
            if ( object != C->obj || (C->obj != NULL && C->obj->getTransparency() > 0) || (C->obj != NULL && C->obj->getReflection() > 0)) {
                object->Intersect(&LocalTuple, D);
                if (LocalTuple.t > 1e-8 && LocalTuple.t < (*C).t) {
                    (*C).normal = LocalTuple.normal;
                    (*C).t      = LocalTuple.t;
                    (*C).obj    = LocalTuple.obj;
                    foundobject = 1;
                }
            }
        }
        return foundobject;
    }
    return 0;
}

bool nTree::Add(Objet *o){
    if (o == NULL)
        return false;

    for(int i=0;i<nbElement;i++)
        if(O[i] == NULL)
            CreateElement(i);

    if( o->isKindOf() == "Triangle" && nbElement == 8){
        OctreePosition positions = ((Triangle *)o)->positionInOctree((Octree*)this);
        
        if(positions.APosition ==positions.BPosition && positions.BPosition == positions.CPosition){
            //Everything in one node
            Add(o,positions.APosition);
            return true;
        }

        if(positions.APosition!=positions.BPosition && positions.BPosition!=positions.CPosition){
            //Tree different nodes
            if(positions.APosition != -1) Add(o,positions.APosition);
            if(positions.BPosition != -1) Add(o,positions.BPosition);
            if(positions.CPosition != -1) Add(o,positions.CPosition);
            return true;
        }
        
        if(positions.APosition==positions.BPosition && positions.BPosition!=positions.CPosition){
            if(positions.APosition != -1)  Add(o,positions.APosition);
            if(positions.CPosition != -1)  Add(o,positions.CPosition);
            return true;
        } 
        if(positions.APosition==positions.CPosition && positions.BPosition!=positions.CPosition){
            if(positions.APosition != -1)  Add(o,positions.APosition);
            if(positions.BPosition != -1)  Add(o,positions.BPosition);
            return true;
        }
        if(positions.BPosition==positions.CPosition && positions.APosition!=positions.CPosition){
            if(positions.APosition != -1)  Add(o,positions.APosition);
            if(positions.BPosition != -1)  Add(o,positions.BPosition);
            return true;
        }
        positions.Show();
        return false;
    }

    int p = CheckObject(o);
    return Add(o,p);
}

bool nTree::Add(Objet* o, int p){
    if (p == -1 || Level == MAX_LEVEL || (LocalCount > 0 && nbElement==8)){
    too_small:
        ObjectList->push_back(o);
        LocalCount++ ;
        if (Root != NULL)
            Root->TotalCount++;
        return true;
    } else {
        if (nbElement == 4) {
            if (((QuadTree *)this)->getWidth() < 8 || ((QuadTree *)this)->getHeight() < 8)
                goto too_small;
        }
        return O[p]->Add(o);
    }
    return true;
}

double nTree::getHeight() {
    return 0;
}
double nTree::getWidth() {
    return 0;
}
double nTree::getDepth() {
    return 0;
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
    return 0;
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

void nTree::Visualize(){
    //if(LocalCount > 0){
        
        for(int i=0; i < Level; i++){
            cout << " ";
        }
        cout << "Level(" << Level << ")(" << Position << ") -- " << LocalCount << endl;
    //}
    for(int i=0;i<nbElement; i++){
        if(O[i] != NULL)
            O[i]->Visualize();
    }
}

nTree* nTree::getChild(int i){
    if(i<0 || i>=nbElement)
        throw;
    
    return O[i];
}

}