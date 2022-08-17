using namespace std;
#include "Core/nTree.hh"
#include "Core/Octree.hh"
#include "Core/Object.hh"

#include <iostream>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include "Droite.hh"
#include "Ray.hh"
#include "Tuple.hh"

namespace Marcel{

	#define EPSILON  1e-8
	#define MAXVALUE 1e8
	#define MIN(a,b)   a>b?b:a
	#define MAX(a,b)   a>b?a:b

	Octree::Octree(Point Bmin, Point Bmax, int _Level):nTree(8){
		BBox.setBoundingMin(Bmin);
		BBox.setBoundingMax(Bmax);
		Level = _Level;
		Root  = NULL;
	}

	Octree::Octree(double xm, double Xm, double ym, double Ym, double zm, double Zm): nTree(8)	{
		Level      = 0;
		Position   = 0;
		Root       = NULL;

		BBox.setBoundingMin(Point(xm,ym,zm));
		BBox.setBoundingMax(Point(Xm,Ym,Zm));
	}

	Octree::Octree(Point _Center,double span): nTree(8) {
		Level    = 0;
		Position = 0;
		Root     = NULL;

		BBox.setBoundingMin(Point(_Center.x - span,_Center.y - span,_Center.z - span));
		BBox.setBoundingMax(Point(_Center.x + span,_Center.y + span,_Center.z + span));
	}

	Octree::Octree(int level, int position, Octree* _Root): nTree(8)	{
		MAX_LEVEL = _Root->MAX_LEVEL;
		Level = level;
		Position = position;
		Root = _Root;

		double xmin,ymin,zmin,xmax,ymax,zmax;

		if (_Root == NULL){ // Creation du premier Octree
			xmin = ymin = zmin = -10000;
			xmax = ymax = zmax =  10000;
		} else {
			switch (position){
			case 0:
				xmin = _Root->getBoundingMin().x;
				xmax = (_Root->getCenter()).x;
				ymin = _Root->getBoundingMin().y;
				ymax = (_Root->getCenter()).y;
				zmin = _Root->getBoundingMin().z;
				zmax = (_Root->getCenter()).z;
				break;
			case 1:
				xmin = (_Root->getCenter()).x;
				xmax = _Root->getBoundingMax().x;
				ymin = _Root->getBoundingMin().y;
				ymax = (_Root->getCenter()).y;
				zmin = _Root->getBoundingMin().z;
				zmax = (_Root->getCenter()).z;
				break;
			case 2:
				xmin = _Root->getBoundingMin().x;
				xmax = (_Root->getCenter()).x;
				ymin = (_Root->getCenter()).y;
				ymax = _Root->getBoundingMax().y;
				zmin = _Root->getBoundingMin().z;
				zmax = (_Root->getCenter()).z;
				break;
			case 3:
				xmin = (_Root->getCenter()).x;
				xmax = _Root->getBoundingMax().x;
				ymin = (_Root->getCenter()).y;
				ymax = _Root->getBoundingMax().y;
				zmin = _Root->getBoundingMin().z;
				zmax = (_Root->getCenter()).z;
				break;
			case 4:
				xmin = _Root->getBoundingMin().x;
				xmax = (_Root->getCenter()).x;
				ymin = _Root->getBoundingMin().y;
				ymax = (_Root->getCenter()).y;
				zmin = (_Root->getCenter()).z;
				zmax = _Root->getBoundingMax().z;
				break;
			case 5:
				xmin = (_Root->getCenter()).x;
				xmax = _Root->getBoundingMax().x;
				ymin = _Root->getBoundingMin().y;
				ymax = (_Root->getCenter()).y;
				zmin = (_Root->getCenter()).z;
				zmax = _Root->getBoundingMax().z;
				break;
			case 6:
				xmin = _Root->getBoundingMin().x;
				xmax = (_Root->getCenter()).x;
				ymin = (_Root->getCenter()).y;
				ymax = _Root->getBoundingMax().y;
				zmin = (_Root->getCenter()).z;
				zmax = _Root->getBoundingMax().z;
				break;
			case 7:
				xmin = (_Root->getCenter()).x;
				xmax = _Root->getBoundingMax().x;
				ymin = (_Root->getCenter()).y;
				ymax = _Root->getBoundingMax().y;
				zmin = (_Root->getCenter()).z;
				zmax = _Root->getBoundingMax().z;
				break;
			}
		}
		BBox.setBoundingMin(Point(xmin,ymin,zmin));
		BBox.setBoundingMax(Point(xmax,ymax,zmax));
	}

	void Octree::CreateElement(int i) {
			O[i] = new Octree(Level + 1, i, this);
	}

	Octree::~Octree()
	{
		if (ObjectList != NULL)
			delete ObjectList;

		for (int i = 0; i < nbElement; i++)
			if (O[i] != NULL)
				delete O[i];
	}

	int Octree::ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob)
	{
		int found = 0;

		if (TestIntersection(D) == true){
			if (LocalCount > 0){
				Tuple LocalTuple;

				for(Objet*& object: (*ObjectList)){
					if (object != Ob){
						object->Intersect(&LocalTuple, D);
						if (LocalTuple.t > EPSILON && LocalTuple.t < t){
							if (LocalTuple.obj->getMaterial() != NULL)
								if (LocalTuple.obj->getMaterial()->getTransparency() == 0) {
									(*NL) = (*NS) = 0;
									return -1;
								}

							found = 1;
							if (LocalTuple.obj->getMaterial() != NULL) {
								(*NL) = (*NL) * LocalTuple.obj->getMaterial()->getTransparency();
								(*NS) = (*NS) * LocalTuple.obj->getMaterial()->getTransparency();
							}
						}
					}
				}
			}

			for (int i = 0; i < nbElement; i++)
				if (O[i] != NULL) // To avoid empty leaves
					if (O[i]->ComputeLight(D, t, NL, NS, Ob) == -1){
						return -1;
					}
		}
		return found;
	}

	bool Octree::TestIntersection(Droite *Dr){
		double t_enter,t_exit;
		return BBox.hit(Dr,t_enter,t_exit);
	}

	void Octree::Deduplicate(){
		if(ObjectList->size() > 0){
			//cout << "Before: " << ObjectList->size() << endl;
			sort( (*ObjectList).begin(), (*ObjectList).end() );
			(*ObjectList).erase( unique( (*ObjectList).begin(), (*ObjectList).end() ), (*ObjectList).end() );
			//cout << "After: " << ObjectList->size() << endl;
		}

		for(int i=0;i<8;i++){
			if(O[i] != NULL){
				((Octree *)O[i])->Deduplicate();
			}
		}
	}

	bool Octree::Add(Objet *o){
		if (o == NULL)
			return false;

		for(int i=0;i<nbElement;i++)
			if(O[i] == NULL)
				CreateElement(i);

		if( o->isKindOf() == "Triangle"){
			OctreePosition positions = positionInChildren((Triangle *)o);
			
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

	bool Octree::Add(Objet* o, int p){
		if (p == -1 || Level == MAX_LEVEL || LocalCount > 0){
			ObjectList->push_back(o);
			LocalCount++ ;
			if (Root != NULL)
				((Octree *)Root)->TotalCount++;
			for(int i=0;i<nbElement;i++){
				if(O[i]!=NULL){
					if(((Octree*)O[i])->LocalCount > 0){
						cout << ((Octree*)O[i])->LocalCount << endl;
					}
				}
			}

			return true;
		} else {
			return O[p]->Add(o);
		}
		return true;
	}

	int Octree::CheckObject(Objet *o){
		Point _min = o->getBoundingBox()->getBoundingMin();
		Point _max = o->getBoundingBox()->getBoundingMax();
		BoundingBox oBbox(_min,_max);

		for(int i=0;i<8;i++){
			if(O[i] != NULL){
				if(((Octree *)O[i])->getBoundingBox().contains(oBbox)){
					return i;
				}
			}
		}
		return -1;
	}

	OctreePosition Octree::positionInChildren(Triangle *T){
		OctreePosition positions;
		
		Point A = T->getA();
		Point B = T->getB();
		Point C = T->getC();

		for(int i=0;i<8;i++){
			if(((Octree *)O[i])->getBoundingBox().contains(A)){
				positions.APosition = i;	
			}
			if(((Octree *)O[i])->getBoundingBox().contains(B)){
				positions.BPosition = i;	
			}
			if(((Octree *)O[i])->getBoundingBox().contains(C)){
				positions.CPosition = i;	
			}
		}

		//positions.Show();
		return positions;
	}

	double Octree::getWidth()  { return fabs(BBox.getBoundingMax().x - BBox.getBoundingMin().x); }
	double Octree::getHeight() { return fabs(BBox.getBoundingMax().y - BBox.getBoundingMin().y); }
	double Octree::getDepth()  { return fabs(BBox.getBoundingMax().z - BBox.getBoundingMin().z); }

	Point  Octree::getBoundingMin(){
		return BBox.getBoundingMin();
	}
	Point  Octree::getBoundingMax(){
		return BBox.getBoundingMax();
	}
	Point Octree::getCenter(){
		return BBox.getCenter();
	}
}