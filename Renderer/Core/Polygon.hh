#ifndef _POLYGON_HH_
#define _POLYGON_HH_

#include "Object.hh"
#include "Triangle.hh"
#include "Octree.hh"
#include "Color.hh"
#include "Point.hh"
#include "Vector.hh"
#include "Droite.hh"
#include "Matrix.hh"

#include "Core/QuadTree.hh"
#include "Core/Image.hh"
#include <vector>

namespace Marcel{

class Polygon : public Objet
{
private:
  vector<Objet *>   *TriangleList;
  Octree*          TriangleOctree;
  QuadTree*        TriangleQuadTree;
public:
        Polygon();
        ~Polygon();
  void  AddTriangle(Triangle *);
  Color getColor(Point *);
  inline Octree* getOctree(){ return TriangleOctree;  }

  int   Intersect(Tuple *,Droite *D);
  int   CreateSmallestOctree(int);
  int   CreateSmallestQuadtree(int);
  void  applyTransformation(Matrix *);

  vector<Objet *>   *getTriangleList();
  void  DrawQuadTree(Image *,int,int);
  bool  isPolygon();
  string isKindOf() { return "Polygon"; }

  void  setMaxOctreeLevel(int);
  void  setMaxQuadtreeLevel(int);
};
}

#endif

