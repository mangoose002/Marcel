#include "Utils/Network/define.hh"
#include "Core/RowDispatcherClient.hh"
#include "Utils/Network/SocketException.hh"

namespace Marcel{

	RowDispatcherClient::RowDispatcherClient(std::string host, int port, Image * aImage,int blockSize, int direction): RowDispatcher(aImage,blockSize,direction){
		int ack;
		socket  = new ClientSocket(host,port);

		//We identify the client type
		socket->send("",_ROWDISPATCHER_CLIENT_);
		ack = socket->recv(4);

		if(ack != _ROWDISPATCHER_CLIENT_ACK_){
			throw SocketException("Unable to connect to RowDispatcherServer");
		}
	}

	RowDispatcherClient::~RowDispatcherClient(){
		delete socket;
	}

	int RowDispatcherClient::nextRow(){
		int line;
		lock_guard<mutex> guard(mMutex);

		socket->send("",_LINE_REQUEST_);
		int response = socket->recv(6,"%w",&line);

		if(response == _LINE_RESPONSE_){
			if(line == MAX_ROW_VALUE){ //We only handle positive numbers
				return -1;
			}
			return line;
		}
		return -1;
	}
}
