using namespace std;
#include "Core/CullingBox.hh"
#include "Core/Ray.hh"

namespace Marcel{
    CullingBox::CullingBox(){   
        xmin = ymin = 0;
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

    bool CullingBox::Test(Droite *R){
        if(R->x < xmin) return false;
        if(R->x > xmax) return false;
        if(R->y < ymin) return false;
        if(R->y > ymax) return false;
 
        return true;
    }

    void CullingBox::setXValues(int x, int X) { xmin = x; xmax = X; }
    void CullingBox::setYValues(int y, int Y) { ymin = y; ymax = Y; }
    int  CullingBox::getXmin()                { return xmin;        }
    int  CullingBox::getYmin()                { return ymin;        }
    int  CullingBox::getXmax()                { return xmax;        }
    int  CullingBox::getYmax()                { return ymax;        }
}
