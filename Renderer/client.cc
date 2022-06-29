
#include "Utils/Utils.hh"
#include "Core/Triangle.hh"
#include "Core/Object.hh"
#include "Core/SceneClient.hh"
#include "Core/Raytrace.hh"

#include "Utils/Network/SocketException.hh"
//#include "Utils/TinyThread/tinythread.h"

double Marcel::Socket::time_spent_recv = 0;
double Marcel::Socket::time_spent_send = 0;
uint64_t Marcel::Objet::SuccessfulIntersectionNumber = 0;
uint64_t Marcel::Objet::IntersectionNumber = 0;



#include <iostream>
#include <thread>
using namespace std;
using namespace Marcel;

///////////////////////////////////////////////////////
int main(int argc, char* argv[]){
	cout << "Running on a system with " << thread::hardware_concurrency() << " cores" << endl;

	SceneContext *context = Utils::ParseCommandLine(argc,argv,_CLIENT_MODE_);

	try{
		Scene *S = new SceneClient(context->hostname, context->port);
		S->Start();
		delete S;
	}
	catch(SocketException &e){
		cout << e.description() << endl;
	}
	catch(...){
		cout << "An error has occured";
	}

	cout << "Receive: " << Socket::time_spent_recv << endl;
	cout << "Send:    " << Socket::time_spent_send << endl;
}
