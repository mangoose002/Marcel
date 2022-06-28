#ifndef _UDP_SOCKET_HH_
#define _UDP_SOCKET_HH_

#include <netinet/in.h>
#include <string>
#include "Utils/Network/Socket.hh"

namespace Marcel{
	class UdpSocket: public Socket{

	protected:
		struct sockaddr_in remaddr;
		socklen_t          addrlen;

	public:
		UdpSocket();
		UdpSocket(int l_port);
		UdpSocket( const std::string host, const int port );

		bool create();


		// Data Transimission
		bool send(const char *message,const int size);
		int recv(char **message,const int size);
	};
}

#endif