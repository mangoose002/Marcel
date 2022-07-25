#include "Core/BoundingBox.hh"

namespace Marcel{
    BoundingBox::BoundingBox(){
        BoundingMin = Point(-5000,-5000,-5000);
        BoundingMax = Point(5000,5000,5000);
        Center = Point(0,0,0);
    }

    BoundingBox::BoundingBox(Point &min, Point &max){
        BoundingMin = min;
        BoundingMax = max;
        Center = min.Middle(max);
    }

    bool BoundingBox::hit(Droite *R,double &t_enter,double &t_exit){
        Point o(R->O);
        Point d(R->D); 
        Point t_min;
        Point t_max;

        double a = 1.0 / d.x;
        if(a >= 0){
            t_min.x = (BoundingMin.x - o.x) * a;
            t_max.x = (BoundingMax.x - o.x) * a; 
        }
        else{
            t_min.x = (BoundingMax.x - o.x) * a;
            t_max.x = (BoundingMin.x - o.x) * a;
        }
        double b = 1.0 / d.y;
        if(b >= 0){
            t_min.y = (BoundingMin.y - o.y) * b;
            t_max.y = (BoundingMax.y - o.y) * b; 
        }
        else{
            t_min.y = (BoundingMax.y - o.y) * b;
            t_max.y = (BoundingMin.y - o.y) * b;
        } 
        double c = 1.0 / d.z;
        if(c >= 0){
            t_min.z = (BoundingMin.z - o.z) * c;
            t_max.z = (BoundingMax.z - o.z) * c; 
        }
        else{
            t_min.z = (BoundingMax.z - o.z) * c;
            t_max.z = (BoundingMin.z - o.z) * c;
        }
        // finding largest
        if(t_min.x > t_min.y){
            t_enter = t_min.x;
        }
        else{
            t_enter = t_min.y;
        }
        if(t_min.z > t_enter){
            t_enter = t_min.z;
        }
        // find smallest
        if(t_max.x < t_max.y){
            t_exit = t_max.x;
        }
        else{
            t_exit = t_max.y;
        }
        if(t_max.z < t_exit){
            t_exit = t_max.z;
        }
        return (t_enter < t_exit && t_exit > 1e-8);
    }

    bool BoundingBox::contains(Point &P){
        return ((P.x > BoundingMin.x && P.x < BoundingMax.x) 
         && (P.y > BoundingMin.y && P.y < BoundingMax.y) 
         && (P.z > BoundingMin.z && P.z < BoundingMax.z));   
    }
}