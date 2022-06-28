using namespace std;

#include <iostream>
#include <string>
#include <string.h>

#include "Core/ImageClient.hh"

#include "Utils/Network/define.hh"
#include "Utils/Network/SocketException.hh"

namespace Marcel{
ImageClient::ImageClient(string host, int port) {
	int ack, x, y;
	char filename[256];

	socket  = new ClientSocket(host, port);

	//We identify the client type
	socket->send("", _IMAGE_CLIENT_);

	ack = socket->recv(0,"%w%w%s", &x, &y, &filename);
	if (ack != _IMAGE_CLIENT_ACK_) {
		throw SocketException("Unable to connect to RowDispatcherServer");
	} else {
		cout << filename << endl;
		cout << "Image(" << x << "x" << y << ")" << endl;

		XResolution = x;
		YResolution = y;
		this->Filename = filename;

		File = new Color*[x];
		for (int i = 0; i < x; i++)
			File[i] = new Color[y];

		socket->ack(_IMAGE_CLIENT_SIZE_ACK_);
	}
}

void ImageClient::saveLine(int y) {
	lock_guard<mutex> lock(mMutex);

	char buffer[16384];
	socket->getHandler()->CreateMessage("%w", _LINE_RESULT_, y);

	for (int i = 0; i < XResolution; i++) {
		socket->getHandler()->setColor(i, this->getColor(i, YResolution - y - 1));
	}

	memcpy(buffer, socket->getHandler()->getSendBuffer(), 16384);
	((Socket*)socket)->send( buffer, 4 + 2 + 3 * XResolution);
}

void ImageClient::saveImage() {
	cout << "Nothing to do" << endl;	//Nothing to do for client/server image
}
}
