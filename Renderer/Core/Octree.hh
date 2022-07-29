#ifndef _OCTREE_HH_
#define _OCTREE_HH_

#include "Core/nTree.hh"
#include "Core/Object.hh"
#include "Core/Droite.hh"
#include "Core/Tuple.hh"
#include "Core/Plan.hh"
#include "Core/Triangle.hh"

namespace Marcel {

	enum NodePosition
	{
		NEAR_BOTTOM_LEFT  = 4,
		NEAR_BOTTOM_RIGHT = 0,
		NEAR_TOP_LEFT     = 6,
		NEAR_TOP_RIGHT    = 2,

		FAR_BOTTOM_LEFT   = 5,
		FAR_BOTTOM_RIGHT  = 1,
		FAR_TOP_RIGHT     = 3,
		FAR_TOP_LEFT      = 7,
	};

	struct OctreePosition{
		int APosition;
		int BPosition;
		int CPosition;

		OctreePosition(){
			APosition = BPosition = CPosition = -1;
		}

		void Show(){
			cout << "----------------" << endl;
			cout << "A: " << APosition << endl;
			cout << "B: " << BPosition << endl;
			cout << "C: " << CPosition << endl;
		}
	};

	class Octree : public nTree
	{
	private:
		BoundingBox BBox;

		bool  TestIntersection(Droite *D);
		int  CheckObject(Objet *);
		OctreePosition positionInChildren(Triangle *);
		void CreateElement(int);

	public:
		Octree(int, int, Octree *);
		Octree(Point,Point,int=0);
		Octree(double, double, double, double, double, double);
		Octree(Point,double);
		~Octree();

		bool Add(Objet *);
		bool Add(Objet *,int);

		int  ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob);
		void Deduplicate();

		double getHeight();
		double getWidth();
		double getDepth();
		BoundingBox getBoundingBox() { return BBox; }

		Point  getCenter();
		Point  getBoundingMin();
		Point  getBoundingMax();
	};
}

#endif
