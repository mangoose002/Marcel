#ifndef _NTREE_HH_
#define _NTREE_HH_

#include "Object.hh"
//#include "Core/Triangle.hh"
//#include "Core/Polygon.hh"

#include "Droite.hh"
#include "Tuple.hh"
#include "Plan.hh"
#include "Core/Image.hh"

#include <vector>

namespace Marcel{
	class Polygon;
	class Triangle;

	class nTree {
		protected:
			int            Level;
			int            Position;
			int            nbElement;
			int            LocalCount;
			int            TotalCount;
			int            MAX_LEVEL;

			nTree*         Root;
			nTree**        O;
			vector<Objet *>* ObjectList;

			int           getCount();
			int           getCumulativeCount();
			virtual bool  TestIntersection(Droite *D)  = 0;
			virtual int   CheckObject(Objet *)         = 0;
			virtual void  CreateElement(int)           = 0;

		public:
			nTree(int);
			~nTree();

			void setMaxLevel(int l);

			virtual bool Add(Objet *) = 0;
			virtual bool Add(Objet *,int) = 0;

			void Visualize();
			int Intersect(Droite *D, Tuple *C);
			virtual int ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob);

			void UpdateCount(int *);
			int  getTotalCount();
			virtual void Draw(Image*, int, int, Color);
			virtual double getHeight();
			virtual double getWidth();
			virtual double getDepth();

			nTree* getChild(int i);
	};
}
#endif
