#ifndef _ANIMATION_HH_
#define _ANIMATION_HH_

#include "Matrix.hh"
#include "KeyFrame.hh"

namespace Marcel{

  class Animation
  {
  private:
    int FrameNumber;
    int KeyFrameNumber;

    Matrix **Animate;

  public:
    Animation();
    Animation(int);
    int addKeyFrame(KeyFrame *);
    int setFrameNumber(int);

    int getFrameNumber();
    int getKeyFrameNumber();
    Matrix* getTransformation(int);
  };
}

#endif
