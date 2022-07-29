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
        Root       = NULL;

        Level      = 0;
        Position   = 0;
        MAX_LEVEL  = 0;
    }

    QuadTree::QuadTree(int level, int position, QuadTree* _Root): nTree(4){
        MAX_LEVEL  = _Root->MAX_LEVEL;
        Level      = level;
        Position   = position;
        Root       = _Root;

        if (_Root == NULL){ // Creating first Quadtree
            xmin = -2000; xmax =  2000;
            ymin = -2000; ymax =  2000;
        } else {
            switch (position) {
            case 0:
                xmin = _Root->xmin;
                xmax = (_Root->xmax + _Root->xmin) / 2;
                ymin = _Root->ymin;
                ymax = (_Root->ymax + _Root->ymin) / 2;
                break;
            case 1:
                xmin = (_Root->xmin + _Root->xmax) / 2;
                xmax = _Root->xmax;
                ymin = _Root->ymin;
                ymax = (_Root->ymax + _Root->ymin) / 2;
                break;
            case 2:
                xmin = _Root->xmin;
                xmax = (_Root->xmax + _Root->xmin) / 2;
                ymin = (_Root->ymax + _Root->ymin) / 2;
                ymax = _Root->ymax;
                break;
            case 3:
                xmin = (_Root->xmin + _Root->xmax) / 2;
                xmax = _Root->xmax;
                ymin = (_Root->ymax + _Root->ymin) / 2;
                ymax = _Root->ymax;
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

    bool QuadTree::Add(Objet *o){
		if (o == NULL)
			return false;

		for(int i=0;i<nbElement;i++)
			if(O[i] == NULL)
				CreateElement(i);

		int p = CheckObject(o);
		return Add(o,p);
	}

	bool QuadTree::Add(Objet* o, int p){
		if (p == -1 || Level == MAX_LEVEL){
			ObjectList->push_back(o);
			LocalCount++ ;
			
			return true;
		} else {
			return O[p]->Add(o);
		}
		return true;
	}

    double QuadTree::getWidth()  { return abs(xmax - xmin); }
    double QuadTree::getHeight() { return abs(ymax - ymin); }
}