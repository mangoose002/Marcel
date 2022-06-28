#ifndef _TEXTURE_HH_
#define _TEXTURE_HH_

#include "Color.hh"

namespace Marcel{

  #define PPM 1
  #define PGM 2
  #define JPG 3

  class Texture{
  protected:
    int           XTiling;
    int           YTiling;
    int           XSize;
    int           YSize;
    double        Strength;
    char          ID[256];
    char          FileName[256];
    int           MaxColorValue;
    int           Animation;
    Color         **map;
    bool          _isLoaded;

  public:
    void           setTiling(int,int);
    void           setUscale(int);
    void           setVscale(int);
    void           setStrength(double);
    void           setAnimate();
    
    virtual Color  getColor(double,double)=0;
    virtual Color  getColor(int,int)=0;
    char          *getID();
    double         getGray(double,double);
    double         getGray(int,int);
    bool           isLoaded();

    virtual void   Animate(int)=0;
    void           ConvertCoordinates(double *u, double *v);

    virtual       ~Texture(){};
  };
}
#endif
