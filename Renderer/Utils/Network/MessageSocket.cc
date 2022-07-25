#include <stdio.h>
#include <string.h>
#include "Utils/Network/define.hh"
#include "Socket.hh"
#include "Utils/MessageHandler.hh"
#include "Utils/Network/MessageSocket.hh"
#include "Utils/Network/SocketException.hh"
#include "Utils/Utils.hh"

namespace Marcel{

MessageSocket::MessageSocket(): Socket() {
	handler = new MessageHandler;
	buffer_size = 16384;
}

MessageSocket::MessageSocket(int port): Socket(port) {
	handler = new MessageHandler;
	buffer_size = 16384;
}

MessageSocket::~MessageSocket() {
	delete handler;
}

void MessageSocket::setBufferSize(int size) {
	buffer_size = size;
}

MessageHandler * MessageSocket::getHandler() {
	return handler;
}

int MessageSocket::send(const char *format, int message_type, ...) {
	//int nbarguments = strlen(format) / 2;
	va_list ap;

	va_start(ap, message_type);
	int size = handler->CreateMessage((char **)&buffer, format, message_type, ap);
	va_end(ap);

	int bytes = Socket::send(buffer, size);

	return bytes;
}

int MessageSocket::recv(int size, const char *format, ...) {
	int bytes;
	//int nbarguments = strlen(format) / 2;
	va_list ap;

	int total = 0;
	if (size == 0) {
		total = Socket::recv((char **)&buffer, 16384);
	} else {
		while (size - total > 0) {
			bytes = Socket::recv((char **)&buffer[total], size - total);
			total += bytes;
		}
	}

	if (total > 0) {
		va_start(ap, format);
		int type = handler->ReadMessage((char *)buffer, total, format, ap);
		va_end(ap);

		return type;
	}

	return bytes;
}

int MessageSocket::ack(int msg_type){
	return send("",msg_type);
}

string MessageSocket::uniqid() {
	char uid[16];
	struct timeval tv;
	int sec, usec;

	gettimeofday((struct timeval *) &tv, (struct timezone *) NULL);
	sec = (int) tv.tv_sec;
	usec = (int) (tv.tv_usec % 0x100000);
	sprintf(uid, "%s%i%08x%05x", "", m_sock, sec, usec);

	return uid;
}

void MessageSocket::sendFile(string Filename) {
	int size,type;
	unsigned char filebuffer[2048];

	FILE* F = fopen(Filename.c_str(), "rb");

	while (!feof(F)) {
		bzero((char *)filebuffer, 2048);

		size = fread(filebuffer, 1, 2048, F);
		handler->CreateMessage("", _RESPONSE_FILE_BLOCK_);
		handler->setBinaryBlock(6, size, filebuffer);

		memcpy(buffer, handler->getSendBuffer(), 16384);
		Socket::send( buffer, 4 + 2 + 2048*2);

		type = recv(4);
		if(type != _REQUEST_FILE_BLOCK_){
			throw new SocketException("Protocol error while sending file");
		}
	}

	fclose(F);
}

string MessageSocket::recvFile(int filesize) {

	int type;
	int ReceivedBytes=0;
	unsigned char filebuffer[2048];
	string output = uniqid();

	FILE* F = fopen(output.c_str(), "wb");
	while (ReceivedBytes < filesize) {
		bzero((char *)filebuffer, 2048);

		type = recv(4 + 2 + 2048*2,"");
		if(type == _RESPONSE_FILE_BLOCK_){
			handler->getBinaryBlock(6,2048,(unsigned char **)&filebuffer);

			if( filesize - ReceivedBytes > 2048){
				fwrite(filebuffer,1,2048,F);
				ReceivedBytes +=2048;
			} else {
				fwrite(filebuffer,1,filesize - ReceivedBytes,F);
				ReceivedBytes += (filesize - ReceivedBytes);
			}

			send("",_REQUEST_FILE_BLOCK_);
		}
	}
	fclose(F);

	return output;
}
}