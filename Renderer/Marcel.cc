
#include "Utils/Utils.hh"
#include "Core/Triangle.hh"
#include "Core/Scene.hh"
#include "Core/Raytrace.hh"

#include <iostream>
#include <thread>
using namespace std;
using namespace Marcel;



//       z
//       |
//       |
//       + ---- y
//      /
//     /
//    x

///////////////////////////////////////////////////////
int main(int argc, char* argv[]){
	cout << "Running on a system with " << thread::hardware_concurrency() << " cores" << endl;

	SceneContext *context = Utils::ParseCommandLine(argc,argv,_LOCAL_MODE_);

	Scene scene(context);
	scene.Start();
}


