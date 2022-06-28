#ifndef _MESSAGE_SOCKET_
#define _MESSAGE_SOCKET_

#include <string>
#include "Socket.hh"
#include "Core/Image.hh"
#include "Utils/MessageHandler.hh"

namespace Marcel{

class MessageSocket: protected Socket{
public:
	MessageSocket();
	MessageSocket(int);
	virtual ~MessageSocket();

	void setBufferSize(int);
	int send(const char *format, int message_type, ...);
	int recv(int size,const char *format="",...);
	int ack(int);

	void sendLine(Image *,int);
	void sendFile(string);

	void recvLine(Image *,int);
	string recvFile(int);

	MessageHandler * getHandler();
private:
	string uniqid();

	MessageHandler *handler;
	char buffer[16384];
	int  buffer_size;
};

}
#endif