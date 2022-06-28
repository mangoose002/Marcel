#ifndef _IMAGE_HH_
#define _IMAGE_HH_

#include <string>
#include "Color.hh"

namespace Marcel{
  class Image
  {
  protected:
    Color  **File;
    int      XResolution;
    int      YResolution;
    string   Filename;
    string   FilePart;
    Image() {};

  public:
    Image(const char *,int,int);
    ~Image();
    void  setColor(Color,int,int);
    void  setBlock(Color,int,int,int);
    Color getColor(int,int);
    void setOutputFile(const char *);
    void setFrame(int);
    float ComputeMaxDifference(int block_size, int x, int y);

    virtual void saveImage();
    virtual void saveLine(int);

    inline int Height(){ return YResolution; }
    inline int Width(){ return XResolution; }
    inline std::string getOuputFilename(){ return Filename; }

  };
}

#endif
