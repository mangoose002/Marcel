#ifndef _COLORTEXTURE_HH_
#define _COLORTEXTURE_HH_

#include "Core/Texture.hh"
#include "Core/Color.hh"
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <string>

namespace Marcel{
class ColorTexture : public Texture
{
public:
  ColorTexture();
  ColorTexture(char *,string, int=1, int=1);
  ~ColorTexture();

  void  Animate(int);
  Color getColor(int,int);
  Color getColor(double,double);
  void LoadFile(string);
};
}
#endif
