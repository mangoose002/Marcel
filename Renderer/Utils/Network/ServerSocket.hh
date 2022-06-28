// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "MessageSocket.hh"
#include "Utils/MessageHandler.hh"

namespace Marcel{

class ServerSocket : public MessageSocket
{
public:
	ServerSocket() {};
	ServerSocket ( int port );

	bool accept(Socket *);
};

}

#endif