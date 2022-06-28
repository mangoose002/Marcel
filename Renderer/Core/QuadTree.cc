using namespace std;
#include "Core/QuadTree.hh"
#include "Core/CullingBox.hh"

#include <stdlib.h>
#include <vector>
#include <algorithm>

namespace Marcel{

    #define MIN(a,b)  a>b?b:a
    #define MAX(a,b)  a>b?a:b

    QuadTree::QuadTree(int x, int X, int y, int Y): nTree(4){
        xmin       = x;        xmax       = X;
        ymin       = y;        ymax       = Y;
        F          = NULL;

        Level      = 0;
        Position   = 0;
        MAX_LEVEL  = 0;
    }

    QuadTree::QuadTree(int level, int position, QuadTree* Father): nTree(4){
        MAX_LEVEL  = Father->MAX_LEVEL;
        Level      = level;
        Position   = position;
        F          = Father;

        if (Father == NULL){ // Creation du premier Octree
            xmin = -2000; xmax =  2000;
            ymin = -2000; ymax =  2000;
        } else {
            switch (position) {
            case 0:
                xmin = Father->xmin;
                xmax = (Father->xmax + Father->xmin) / 2;
                ymin = Father->ymin;
                ymax = (Father->ymax + Father->ymin) / 2;
                break;
            case 1:
                xmin = (Father->xmin + Father->xmax) / 2;
                xmax = Father->xmax;
                ymin = Father->ymin;
                ymax = (Father->ymax + Father->ymin) / 2;
                break;
            case 2:
                xmin = Father->xmin;
                xmax = (Father->xmax + Father->xmin) / 2;
                ymin = (Father->ymax + Father->ymin) / 2;
                ymax = Father->ymax;
                break;
            case 3:
                xmin = (Father->xmin + Father->xmax) / 2;
                xmax = Father->xmax;
                ymin = (Father->ymax + Father->ymin) / 2;
                ymax = Father->ymax;
                break;
            }
        }
    }

    void QuadTree::CreateElement(int i) {
        O[i] = new QuadTree(Level + 1, i, this);
    }

    QuadTree::~QuadTree(){
        if (ObjectList != NULL)
            delete ObjectList;

        for (int i = 0; i < nbElement; i++)
            if (O[i] != NULL)
                delete O[i];
    }

    bool QuadTree::TestIntersection(Droite *R){
        if ((R->x >= xmin && R->x <= xmax && R->y >= ymin && R->y <= ymax))
            return true;

        return false;
    }

    int QuadTree::CheckObject(Objet *o){
        CullingBox *C = o->getCullingBox();
        if (C == NULL)
            return -1;

        double Xmin = xmin;
        double Xmax = (xmax + xmin) / 2;
        double Ymin = ymin;
        double Ymax = (ymax + ymin) / 2;
        if (C->getXmin() >= Xmin && C->getXmax() <= Xmax && C->getYmin() >= Ymin && C->getYmax() <= Ymax)
            return 0;

        Xmin = (xmin + xmax) / 2;
        Xmax = xmax;
        Ymin = ymin;
        Ymax = (ymax + ymin) / 2;
        if (C->getXmin() >= Xmin && C->getXmax() <= Xmax && C->getYmin() >= Ymin && C->getYmax() <= Ymax)
            return 1;

        Xmin = xmin;
        Xmax = (xmax + xmin) / 2;
        Ymin = (ymax + ymin) / 2;
        Ymax = ymax;
        if (C->getXmin() >= Xmin && C->getXmax() <= Xmax && C->getYmin() >= Ymin && C->getYmax() <= Ymax)
            return 2;

        Xmin = (xmin + xmax) / 2;
        Xmax = xmax;
        Ymin = (ymax + ymin) / 2;
        Ymax = ymax;
        if (C->getXmin() >= Xmin && C->getXmax() <= Xmax && C->getYmin() >= Ymin && C->getYmax() <= Ymax)
            return 3;

        return -1;
    }

    double QuadTree::getWidth()  { return abs(xmax - xmin); }
    double QuadTree::getHeight() { return abs(ymax - ymin); }
}