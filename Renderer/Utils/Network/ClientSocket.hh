// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "MessageSocket.hh"
#include "Utils/MessageHandler.hh"

namespace Marcel{

	class ClientSocket : public MessageSocket
	{
	public:
		ClientSocket ( std::string host, int port );
	};
}

#endif