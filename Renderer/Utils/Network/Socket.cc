// Implementation of the Socket class.

#include "Utils/Network/Socket.hh"
#include "Utils/Network/SocketException.hh"

#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

//using namespace tthread;
namespace Marcel{

Socket::Socket() {
	mode = SOCKET_CLIENT;
	m_sock = -1;
	memset(&m_addr, 0, sizeof(m_addr));
}

Socket::Socket(int l_sock) {
	mode = SOCKET_CLIENT;
	m_sock = l_sock;
	memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket() {
	close( m_sock );
}

bool Socket::create(){
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1) {
		throw new SocketException("Unable to create socket");
	}

	return true;
}

bool Socket::bind ( const int port ){
	mode = SOCKET_SERVER;

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind (m_sock, (struct sockaddr *) &m_addr, sizeof (m_addr));

	if ( bind_return == -1 ) {
		throw SocketException("Unable to bind socket");
	}

	return true;
}


bool Socket::listen() {
	mode = SOCKET_SERVER;

	int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

	if ( listen_return == -1 ) {
		throw SocketException("Unable to lisken on socket");
	}

	return true;
}


bool Socket::accept (Socket *sock) {
	mode = SOCKET_SERVER;

	int addr_length = sizeof ( m_addr );
	int l_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

	if ( l_sock <= 0 )
		throw SocketException("Unable to accept socket");
	else{
		sock->m_sock = l_sock;
		return true;
	}
}

bool Socket::send(const char* s, int size){
	//lock_guard<mutex> lock(mMutex);
	timeval *dm = new timeval;
    timeval *fm = new timeval;

    gettimeofday(dm, NULL);
  	int bytes = ::send(m_sock,s,size,0);

	if (bytes < 0)
		throw SocketException("Unable to write on socket");
	else{
		gettimeofday(fm, NULL);
		Socket::time_spent_send += (fm->tv_sec * 1000000 + fm->tv_usec - dm->tv_sec * 1000000 - dm->tv_usec);
		delete dm;
		delete fm;
		return bytes;
	}
}
bool Socket::send ( const std::string s ){
	send(s.c_str(),s.size());
}

int Socket::recv(char **message, const int size){
	//lock_guard<mutex> lock(mMutex);
    timeval *dm = new timeval;
    timeval *fm = new timeval;


    gettimeofday(dm, NULL);
  	int bytes = ::recv(m_sock,message,size,0);

	if (bytes < 0){
		throw SocketException("Unable to read on socket");
	} else if(bytes == 0){
		throw SocketException("Peer is disconnected");
	}
	else {
		gettimeofday(fm, NULL);
		Socket::time_spent_recv += (fm->tv_sec * 1000000 + fm->tv_usec - dm->tv_sec * 1000000 - dm->tv_usec);
		delete dm;
		delete fm;

		return bytes;
	}
}
int Socket::recv ( std::string& s ) {
	char buf[16384];
	memset ( buf, 0, 16384 );

  	int bytes = recv((char **)&buf, 16384);

	if (bytes > 0){
		s = buf;
		return bytes;
	}
}

bool Socket::connect ( const std::string host, const int port ) {

	struct hostent *   Machine;

	//if (!isdigit(host[0])){
		Machine = gethostbyname(host.c_str());
	//else
	//	Machine = gethostbyaddr(host.c_str(), host.size(), AF_INET);

	if (Machine == NULL) {
		throw SocketException("Unable to resolve " + host);
	}

	memset(&m_addr, 0, sizeof(struct sockaddr_in));
	memcpy(&m_addr.sin_addr, Machine->h_addr, Machine->h_length);

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	int status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if(mode == SOCKET_CLIENT){
		int opt=1;
		if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, &opt,sizeof(int))!=0){
			throw new SocketException("Unable to create socket");
		}
	}

	if ( status == 0 )
		return true;
	else
		throw SocketException("Unable to connect to server");
}

void Socket::set_non_blocking ( const bool b ) {
	int opts;

	opts = fcntl ( m_sock, F_GETFL );
	if ( opts < 0 ) {
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl (m_sock, F_SETFL, opts );
}
}
