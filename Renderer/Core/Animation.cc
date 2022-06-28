using namespace std;
#include "Animation.hh"

namespace Marcel{

  Animation::Animation(){
    FrameNumber = KeyFrameNumber = 0;
    Animate = NULL;
  }

  Animation::Animation(int n){
    FrameNumber = n;
    KeyFrameNumber = 0;
    Animate = new Matrix*[n];
      for(int i=0;i<n;i++)
        Animate[i] = new Matrix(4,4);
  }

  int Animation::setFrameNumber(int n){
    if(Animate!=NULL)
      delete Animate;

    Animate = new Matrix*[n]; //(4,4);
    for(int i=0;i<n;i++)
      {
        Animate[i] = new Matrix(4,4);
        Animate[i]->Identity();
      }

    return (FrameNumber = n);
  }

  int Animation::addKeyFrame(KeyFrame *k){
    if(k->getStart()>FrameNumber || k->getStart()<0)
      return 0;

    int FrameStart = k->getStart();
    int FrameEnd;
    if(k->getEnd()>FrameNumber)
      FrameEnd = FrameNumber;
    else
      FrameEnd = k->getEnd();

    for(int i=FrameStart;i<FrameEnd;i++)
      (*Animate[i])=(*Animate[i])*k->getTransformation();

    return 1;
  }

  int Animation::getFrameNumber(){
    return FrameNumber;
  }

  int Animation::getKeyFrameNumber(){
    return KeyFrameNumber;
  }

  Matrix* Animation::getTransformation(int i){
    return Animate[i];
  }
}