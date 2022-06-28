#include "Utils/Network/UdpSocket.hh"
#include "Utils/Network/SocketException.hh"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace Marcel;

int main(void){
	int msgcnt = 0;			/* count # of messages we received */
	char buf[2048];	/* receive buffer */
	UdpSocket server;

	server.create();
	server.bind(21234);

	try {
		for (;;) {
			int recvlen = server.recv((char **)&buf,2048);

			if (recvlen > 0) {
				buf[recvlen] = 0;
				printf("received message: \"%s\" (%d bytes)\n", buf, recvlen);
			}
			else
				printf("uh oh - something went wrong!\n");

			sprintf(buf, "ack %d", msgcnt++);
			printf("sending response \"%s\"\n", buf);
			server.send(buf,strlen(buf));
		}
	} catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
	}
}