#ifndef _BOUNDINGBOX_HH_
#define _BOUNDINGBOX_HH_

#include "Core/Point.hh"
#include "Core/Droite.hh"

namespace Marcel{
    class BoundingBox{
        private:
            Point BoundingMin;
            Point BoundingMax;
            Point Center;
        public:
            BoundingBox();
            BoundingBox(Point&,Point&);

            BoundingBox(const BoundingBox &b) = default;
            ~BoundingBox() = default;

            bool hit(Droite*,double&,double&);
            bool contains(Point&);
            bool contains(BoundingBox&);

            void setBoundingMin(Point min) { BoundingMin = min; Center = BoundingMin.Middle(BoundingMax); }
            void setBoundingMax(Point max) { BoundingMax = max; Center = BoundingMin.Middle(BoundingMax); }

            Point getBoundingMin(){ return BoundingMin; }
            Point getBoundingMax(){ return BoundingMax; }
            Point getCenter()     { return Center;      }
            
            void  show();
        
    };
}
#endif