#include "Core/RowDispatcher.hh"
#include <mutex>
#include <thread>

using namespace std;

namespace Marcel{

	//using namespace tthread;
	RowDispatcher::RowDispatcher(Image * aImage, int blockSize, int direction) {
		mBlockSize = blockSize;
		mImage = aImage;
		mDirection = direction;

		if (mDirection == DISPATCHER_DSC)
			mNextRow = aImage->Height() - 1;
		else
			mNextRow = 0;
	}

	int RowDispatcher::nextRow() {
		lock_guard<mutex> guard(mMutex);
		//this_thread::sleep_for(chrono::microseconds(100));

		if (mDirection == DISPATCHER_DSC) {
			if (mNextRow <= 0)
				return -1;
		} else {
			if (mNextRow > mImage->Height()) {
				return -1;
			}
		}

		int result = mNextRow;
		mNextRow += mBlockSize * mDirection;
		return result;
	}

	RowDispatcher::~RowDispatcher() {
	}

	Image * RowDispatcher::getImage() {
		return mImage;
	}

	void RowDispatcher::reset() {
		lock_guard<mutex> guard(mMutex);

		mNextRow = mImage->Height() - 1;
	}

	void RowDispatcher::setBlockSize(int blockSize) {
		mBlockSize = blockSize;
	}
	void RowDispatcher::setDirection(int direction) {
		mDirection = direction;

		if (mDirection == DISPATCHER_DSC)
			mNextRow = mImage->Height() - 1;
		else
			mNextRow = 0;
	}
	int RowDispatcher::getBlockSize() {
		return mBlockSize;
	}
}
