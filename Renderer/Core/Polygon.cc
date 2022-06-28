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
}

Color Polygon::getColor(Point *P) {
	return Colour;
}

int Polygon::Intersect(Tuple *C, Droite *D) {
	//  Debug debug = Debug("Polygon::Intersect");
	(*C).t = 1e8;

	if (D->Level > 0)
		TriangleOctree->Intersect(D, C); //, &O2, NULL);
	else
		TriangleQuadTree->Intersect(D, C); //, &O2, NULL);

	return 1;
}

int Polygon::CreateSmallestOctree(int level) {
	x = y = z =  5000;
	X = Y = Z = -5000;

	Objet *m;
	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);

		if (m != NULL){
			x = MIN(x,m->x);
			X = MAX(X,m->X);
			y = MIN(y,m->y);
			Y = MAX(Y,m->Y);
			z = MIN(z,m->z);
			Z = MAX(Z,m->Z);
		}
	}

	TriangleOctree = new Octree(x, X, y, Y, z, Z);
	TriangleOctree->setMaxLevel(level);

	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);
		if (m != NULL)
			TriangleOctree->Add(m);
	}
	int c = 0;
	TriangleOctree->UpdateCount(&c);

	return 1;
}

int Polygon::CreateSmallestQuadtree(int level) {
	int xm = 5000;
	int Xm = -5000;
	int ym = 5000;
	int Ym = -5000;

	CullingBox *C;

	Objet *m;

	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);
		if (m != NULL)
		{
			C = m->getCullingBox();

			if (C->getXmin() < xm)
				xm = C->getXmin();

			if (C->getXmax() > Xm)
				Xm = C->getXmax();

			if (C->getYmin() < ym)
				ym = C->getYmin();

			if (C->getYmax() > Ym)
				Ym = C->getYmax();
		}
	}

	TriangleQuadTree = new QuadTree(xm, Xm, ym, Ym);
	TriangleQuadTree->setMaxLevel(level);

	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);
		if (m != NULL)
			TriangleQuadTree->Add(m);
	}

	int c = 0;
	TriangleQuadTree->UpdateCount(&c);
	return 1;
}

void Polygon::applyTransformation(Matrix *M) {
	Objet *m;
	for (int yy = 0; yy < TriangleList->size(); yy++) {
		m = TriangleList->at(yy);
		if (m != NULL)
			m->applyTransformation(M);
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

