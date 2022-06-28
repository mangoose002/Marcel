#ifndef _NTREE_H_
#define _NTREE_H_

#include "Object.hh"
#include "Droite.hh"
#include "Tuple.hh"
#include "Plan.hh"
#include "Core/Image.hh"

#include <vector>
namespace Marcel{

class nTree {
protected:
	int            Level;
	int            Position;
	int            nbElement;
	int            LocalCount;
	int            TotalCount;
	int            MAX_LEVEL;

	nTree*         F;
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

	int Add(Objet *);
	int Intersect(Droite *D, Tuple *C);
	virtual int ComputeLight(Droite *D, double t, double *NS, double *NL, Objet *Ob);

	void UpdateCount(int *);
	int  getTotalCount();
	virtual void Draw(Image*, int, int, Color);
	virtual double getHeight();
	virtual double getWidth();
	virtual double getDepth();

};
}
#endif
