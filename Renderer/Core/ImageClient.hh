#ifndef _IMAGE_CLIENT_HH_
#define _IMAGE_CLIENT_HH_

#include <iostream>
#include <mutex>
#include "Core/Image.hh"
#include "Utils/Network/ClientSocket.hh"

//using namespace tthread;
namespace Marcel{
class ImageClient: public Image{
	public:
		ImageClient(std::string,int);
		void saveLine(int);
		void saveImage();
	private:
		ClientSocket   *socket;
		mutex    	    mMutex;
};
}
#endif
