#ifndef _GRAYTEXTURE_HH_
#define _GRAYTEXTURE_HH_

#include "Texture.hh"

namespace Marcel{
class GrayTexture : public Texture
{
private:
  double **map;
  
public:
  GrayTexture(char *,char *, int=1, int=1);
  ~GrayTexture();
  double getGray(int,int);
  double getGray(double,double);

  void  Animate(int);
  Color getColor(int,int);
  Color getColor(double,double);
};
}

#endif
