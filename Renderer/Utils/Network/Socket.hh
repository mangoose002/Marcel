// Definition of the Socket class

#ifndef SOCKET_HH
#define SOCKET_HH

#include <netinet/in.h>
#include <string>

namespace Marcel{

	const int MAXHOSTNAME    = 200;
	const int MAXCONNECTIONS = 5;
	const int MAXRECV        = 500;
	const int SOCKET_SERVER  = 1;
	const int SOCKET_CLIENT  = 2;

	class Socket
	{
	public:
		Socket();
		Socket(int);
		virtual ~Socket();

		static double time_spent_send;
		static double time_spent_recv;

		// Server initialization
		bool create();
		bool bind ( const int port );
		bool listen();
		bool accept(Socket *);

		// Client initialization
		bool connect ( const std::string host, const int port );

		// Data Transimission
		bool send( const std::string ) ;
		bool send(const char *message,const int size);
		int recv(std::string&);
		int recv(char **message,const int size);

		void set_non_blocking ( const bool );

	protected:
		int m_sock;
		int         mode; // 1=Server | 2=Client
		sockaddr_in m_addr;
		//tthread::mutex  mMutex;
	};
}

#endif
