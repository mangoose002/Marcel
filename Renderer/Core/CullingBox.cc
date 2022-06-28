using namespace std;
#include "Core/CullingBox.hh"
#include "Core/Ray.hh"

namespace Marcel{
    CullingBox::CullingBox()
    {   xmin = ymin = 0;
        xmax = ymax = 0;
    }

    CullingBox::CullingBox(int x, int X, int y, int Y) {
        xmin = x;
        ymin = y;
        xmax = X;
        ymax = Y;
    }

    CullingBox::~CullingBox(){
    }

    bool CullingBox::Test(Droite *R)
    {
        if (R->Level > 0)
            return true;

        if (R->x >= xmin && R->x <= xmax && R->y >= ymin && R->y <= ymax)
            return true;


        return false;
    }

    void CullingBox::setXValues(int x, int X) { xmin = x; xmax = X; }
    void CullingBox::setYValues(int y, int Y) { ymin = y; ymax = Y; }
    int  CullingBox::getXmin()                { return xmin;        }
    int  CullingBox::getYmin()                { return ymin;        }
    int  CullingBox::getXmax()                { return xmax;        }
    int  CullingBox::getYmax()                { return ymax;        }
}
