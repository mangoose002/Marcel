#ifndef _ROWDISPATCHER_H_
#define _ROWDISPATCHER_H_

#define DISPATCHER_ASC  1
#define DISPATCHER_DSC -1

#define MAX_ROW_VALUE   65535

using namespace std;

#include "Core/Image.hh"
#include <mutex>

namespace Marcel{
    class RowDispatcher {
    public:
        RowDispatcher(Image * aImage, int blocksize = 1, int direction = DISPATCHER_DSC);
        virtual ~RowDispatcher();
        virtual int nextRow();
        Image * getImage();
        void reset();
        void setBlockSize(int blocksize);
        void setDirection(int);
        int  getBlockSize();

    protected:
        mutex mMutex;
        int mNextRow;
        int mBlockSize;
        int mDirection;
        Image * mImage;
    };
}

#endif
