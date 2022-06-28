#ifndef _SCENE_CLIENT_HH_
#define _SCENE_CLIENT_HH_

#include "Core/Scene.hh"
#include "Utils/Network/ClientSocket.hh"
#include <mutex>

namespace Marcel{

class SceneClient: public Scene{
public:
	SceneClient(string,int);
	~SceneClient();

private:
	ClientSocket   *socket;
	mutex	       mMutex;
};

}

#endif
