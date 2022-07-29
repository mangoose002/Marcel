#ifndef _QUADTREE_HH_
#define _QUADTREE_HH_

#include "Core/nTree.hh"
#include "Core/Image.hh"
#include "Core/Color.hh"

namespace Marcel{

class QuadTree : public nTree
{
private:
	int xmin; // Coordonn�es de l'octree
	int xmax;
	int ymin;
	int ymax;

	bool  TestIntersection(Droite *D);
	int  CheckObject(Objet *);
	void CreateElement(int);

public:
	QuadTree(int, int, QuadTree *);
	QuadTree(int, int, int, int);
	~QuadTree();

	bool Add(Objet *);
	bool Add(Objet *,int);

	double getHeight();
	double getWidth();
};
}

#endif
