// Implementation of the ServerSocket class

using namespace std;

#include "ServerSocket.hh"
#include "SocketException.hh"

namespace Marcel{

ServerSocket::ServerSocket ( int port ){
	if ( ! Socket::create() )
		throw SocketException ( "Could not create server socket." );

	if ( ! Socket::bind ( port ) )
		throw SocketException ( "Could not bind to port." );

    std::cout << "Listening on port " << port << " ..." << std::endl;
	if ( ! Socket::listen() )
		throw SocketException ( "Could not listen to socket." );
}

bool ServerSocket::accept (Socket *s){
	return Socket::accept(s);
}
}