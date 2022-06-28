#include "Utils/Network/UdpSocket.hh"
#include "Utils/Network/SocketException.hh"

#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 2048

namespace Marcel{

	UdpSocket::UdpSocket(){
	}
	UdpSocket::UdpSocket(int l_port): Socket(l_port){
	}

	UdpSocket::UdpSocket( const std::string host, const int port ) {
		mode = SOCKET_CLIENT;
		std::cout << "Create Dgram(1)" << std::endl;

		struct hostent *   Machine;
		Machine = gethostbyname(host.c_str());

		if (Machine == NULL) {
			throw SocketException("Unable to resolve " + host);
		}

		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sock == -1) {
			throw new SocketException("Unable to create socket");
		}

		create();
		bind(0);

		memset(&remaddr, 0, sizeof(struct sockaddr_in));

		remaddr.sin_family = AF_INET;
		memcpy(&remaddr.sin_addr, Machine->h_addr, Machine->h_length);
		remaddr.sin_port = htons ( port );
	}

	bool UdpSocket::create(){
		std::cout << "Create Dgram" << std::endl;
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sock == -1) {
			throw new SocketException("Unable to create socket");
		}

		addrlen = sizeof(remaddr);
		return true;
	}


	bool UdpSocket::send(const char* s, int size){
		std::cout << "Sending dgram" << std::endl;
	  	int bytes = sendto(m_sock, s, size,0,(struct sockaddr *)&remaddr, sizeof(remaddr));

		if (bytes < 0){
			std::cout << strerror(errno) << std::endl;
			throw SocketException("Unable to write on socket");
		}
		else
			return bytes;
	}

	int UdpSocket::recv(char **message,const int size){
	  	int bytes = recvfrom(m_sock, message, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);

		if (bytes < 0){
			std::cout << strerror(errno) << std::endl;
			throw SocketException("Unable to read on socket");
		} else if(bytes == 0){
			std::cout << strerror(errno) << std::endl;
			throw SocketException("Peer is disconnected");
		}
		else {
			return bytes;
		}
	}
}
