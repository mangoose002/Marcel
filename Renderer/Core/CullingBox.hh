#ifndef _CULLINGBOX_HH_
#define _CULLINGBOX_HH_

#include "Core/Droite.hh"

namespace Marcel{
  class CullingBox
  {
  private:
    int xmin;
    int xmax;
    int ymin;
    int ymax;

  public:
    static int NbTests;
    static int NbSuccessful;
    
    CullingBox  ();
    CullingBox  (int,int,int,int);
    ~CullingBox ();
    
    void  setXValues(int,int);
    void  setYValues(int,int);
    bool  Test   (Droite *);
    
    int   getXmin();
    int   getYmin();
    int   getXmax();
    int   getYmax();
  };
}
#endif
