#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "Core/Object.hh"
#include "Core/nTree.hh"
#include "Core/Droite.hh"
#include "Core/Tuple.hh"
#include "Core/Plan.hh"

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

		MAX_NODE_COUNT = 8 //any defined node should be < 8 && > 0
	};

	class Octree : public nTree
	{
	private:
		Point  BoundingMin;
		Point  BoundingMax;
		Point  Center;       // Octree center
		double R;            // Max Octree Radius

		double xmin; // Coordonnées de l'octree
		double xmax;
		double ymin;
		double ymax;
		double zmin;
		double zmax;
		double X;
		double Y;
		double Z;

		bool  TestIntersection(Droite *D);
		int  CheckObject(Objet *);
		void CreateElement(int);

	public:
		Octree(int, int, Octree *);
		Octree(double, double, double, double, double, double);
		~Octree();

		int  ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob);

		double getHeight();
		double getWidth();
		double getDepth();

		Point  getMinBound();
		Point  getMaxBound();


















	};
}

#endif
