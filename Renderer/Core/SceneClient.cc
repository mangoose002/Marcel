using namespace std;

#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <string.h>

#include "Core/SceneClient.hh"
#include "Core/ImageClient.hh"
#include "Core/RowDispatcherClient.hh"

#include "Utils/Network/define.hh"
#include "Utils/Network/SocketException.hh"

namespace Marcel{

	SceneClient::SceneClient(string host, int port): Scene() {
		int ack;
		int filesize;

		socket  = new ClientSocket(host, port);

		//We identify the client type
		socket->send("", _CONFIG_CLIENT_);

		ack = socket->recv(0, "%w%w%w%w%w%w%w%i",
							&ResX,
						    &ResY,
		                    &antialias,
		                    &Progressive,
		                    &BlockSize,
		                    &Seuil,
		                    &numThreads,
		                    &filesize);
		cout << ack << endl;

		if (ack != _CONFIG_CLIENT_ACK_) {
			throw SocketException("Unable to connect to ConfigServer");
		} else {

			if(numThreads == 65535){
				numThreads = thread::hardware_concurrency();
				if (numThreads < 1)
					numThreads = 1;
			}

			cout << "Antialias: " << antialias << endl;
			cout << "Progressive: " << Progressive << endl;
			cout << "Blocksize: " << BlockSize << endl;
			cout << "Seuil: " << Seuil << endl;
			cout << "Threads: " << numThreads << endl;
			cout << "Size: " << filesize << endl;

			InputFileName = socket->recvFile(filesize);

			File = new ImageClient(host, port);
			Dispatcher = new RowDispatcherClient(host, port, File);
		}
	}

	SceneClient::~SceneClient(){
		unlink(InputFileName.c_str());
	}
}
