#ifndef _TRIANGLE_HH_
#define _TRIANGLE_HH_

#include "Point.hh"
#include "Vector.hh"
#include "Droite.hh"
#include "Tuple.hh"
#include "Object.hh"
#include "Plan.hh"
#include "Color.hh"
#include "Matrix.hh"
#include "Core/Octree.hh"

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

namespace Marcel{
	class Triangle : public Objet
	{
	private:
		Point  A;
		Point  B;
		Point  C;

		//For mapping coordinates
		double UVAx;  double UVAy;
		double UVBx;  double UVBy;
		double UVCx;  double UVCy;

		Droite AB;
		Droite AC;
		Droite BC;

		double abn;
		double acn;
		double bcn;

		Point  Center;
		Vector N;
		Vector N1;

		Vector ShadingNormalA;
		Vector ShadingNormalB;
		Vector ShadingNormalC;

		Vector V1;
		Vector V2;
		double n;
		double R2;

		Plan   P;
		double Area2;

		bool   TestIntersection(Droite *);
		Vector computeShadedNormal(Point*);
	public:
		Triangle(Point, Point, Point);
		~Triangle();

		int    Intersect(Tuple *, Droite *);
		void   applyTransformation(Matrix *);

		void   setShadingNormal(int, Vector);
		void   setUVCoordinates(int, double, double);
		void   defineBoundingBox();

		bool   isTriangle();

		Color  getColor(Point *);
		Vector getNormal(Point *,Point *);
		Point  computeUVW(Point *);
		string isKindOf() { return "Triangle"; }

		OctreePosition positionInOctree(Octree *);
	};
}
#endif
