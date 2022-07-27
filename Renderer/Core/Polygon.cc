using namespace std;
#include "Core/Polygon.hh"
#include "Core/Object.hh"
#include "Core/Octree.hh"
#include "Core/Triangle.hh"
#include "Core/Droite.hh"
#include "Core/Tuple.hh"
#include "Core/Point.hh"

#include <vector>
#include <iostream>

namespace Marcel{

	#define MIN(a,b)   a>b?b:a
	#define MAX(a,b)   a>b?a:b

Polygon::Polygon(): Objet()
{
	TriangleList     = new vector<Objet *>;
	TriangleOctree   = NULL;
	TriangleQuadTree = NULL;

	Point min = Point(5000,5000,5000);
	Point max = Point(-5000,-5000,-5000);

	BBox->setBoundingMin(min);
	BBox->setBoundingMax(max);
}

Polygon::~Polygon()
{
	Objet *m;

	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);
		if (m != NULL)
			delete m;
	}
	delete TriangleList;

	if (TriangleOctree != NULL)
		delete TriangleOctree;

	if (TriangleQuadTree != NULL)
		delete TriangleQuadTree;
} 

void Polygon::AddTriangle(Triangle *T) {
	T->applyTransformation(InitialTransformation);
	TriangleList->push_back(T);

	double x,y,z;
	double X,Y,Z;

	x = MIN(BBox->getBoundingMin().x,T->getBoundingBox()->getBoundingMin().x);
	X = MAX(BBox->getBoundingMax().x,T->getBoundingBox()->getBoundingMax().x);
	y = MIN(BBox->getBoundingMin().y,T->getBoundingBox()->getBoundingMin().y);
	Y = MAX(BBox->getBoundingMax().y,T->getBoundingBox()->getBoundingMax().y);
	z = MIN(BBox->getBoundingMin().z,T->getBoundingBox()->getBoundingMin().z);
	Z = MAX(BBox->getBoundingMax().z,T->getBoundingBox()->getBoundingMax().z);

	Point min = Point(x,y,z);
	Point max = Point(X,Y,Z);
	BBox->setBoundingMin(min);
	BBox->setBoundingMax(max);
}

void Polygon::defineBoundingBox(){
	double x,y,z; x=y=z=5000;
	double X,Y,Z; X=Y=Z=-5000;

	for(Objet*& t: *TriangleList){ 
		x = MIN(x,t->getBoundingBox()->getBoundingMin().x);	X = MAX(X,t->getBoundingBox()->getBoundingMax().x);
		y = MIN(y,t->getBoundingBox()->getBoundingMin().y);	Y = MAX(Y,t->getBoundingBox()->getBoundingMax().y);
		z = MIN(z,t->getBoundingBox()->getBoundingMin().z);	Z = MAX(Z,t->getBoundingBox()->getBoundingMax().z);
	}

	Point min = Point(x,y,z);
	Point max = Point(X,Y,Z);
	BBox->setBoundingMin(min);
	BBox->setBoundingMax(max);
}

Color Polygon::getColor(Point *P) {
	return Colour;
}

int Polygon::Intersect(Tuple *C, Droite *D) {
	(*C).t = 1e8;

	if (D->Level > 0)
		TriangleOctree->Intersect(D, C);
	else
		TriangleQuadTree->Intersect(D, C);

	return 1;
}

int Polygon::CreateSmallestOctree(int level) {
	TriangleOctree = new Octree(BBox->getBoundingMin(),BBox->getBoundingMax());
	TriangleOctree->setMaxLevel(level);

	for(Objet*& t: *TriangleList){ 
		if (t != NULL)
			TriangleOctree->Add((Triangle *)t);
	}
	int c = 0;
	TriangleOctree->UpdateCount(&c);

	//cout << endl;
	//((nTree*)TriangleOctree)->Visualize();

	return 1;
}

int Polygon::CreateSmallestQuadtree(int level) {
	int xm = 5000;
	int Xm = -5000;
	int ym = 5000;
	int Ym = -5000;

	CullingBox *C;

	for(Objet*& m: *TriangleList){
		if (m != NULL)
		{
			C = m->getCullingBox();
			xm = MIN(xm,C->getXmin());
			Xm = MAX(Xm,C->getXmax());

			ym = MIN(ym,C->getYmin());
			Ym = MAX(Ym,C->getYmax());
		}
	}

	TriangleQuadTree = new QuadTree(xm, Xm, ym, Ym);
	TriangleQuadTree->setMaxLevel(level);

	for(Objet*& m: *TriangleList){
		if (m != NULL)
			TriangleQuadTree->Add((Triangle *)m);
	}

	int c = 0;
	TriangleQuadTree->UpdateCount(&c);

	return 1;
}

void Polygon::applyTransformation(Matrix *M) {
	for(Objet*& t: *TriangleList){ 
		if (t != NULL)
			t->applyTransformation(M);
	}

	if (TriangleOctree != NULL)
		delete TriangleOctree;

	if (TriangleQuadTree != NULL)
		delete TriangleQuadTree;

	CreateSmallestOctree(2);
	CreateSmallestQuadtree(2);
}


bool  Polygon::isPolygon() {
	return true;
}
vector<Objet *>* Polygon::getTriangleList() {
	return TriangleList;
}

void  Polygon::DrawQuadTree(Image *I, int X, int Y) {
	if (TriangleQuadTree != NULL)
		TriangleQuadTree->Draw(I, X, Y, Color(1, 0, 0));
}

void  Polygon::setMaxOctreeLevel(int l) {
	if (TriangleOctree != NULL)
		TriangleOctree->setMaxLevel(l);
}

void  Polygon::setMaxQuadtreeLevel(int l) {
	if (TriangleQuadTree != NULL)
		TriangleQuadTree->setMaxLevel(l);
}

}

