#include "Utils/Network/UdpSocket.hh"
#include "Utils/Network/SocketException.hh"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace Marcel;

int main(void){
	char buf[2048];	/* receive buffer */

	UdpSocket *client = new UdpSocket("localhost",21234);

	try {
		for (int i=0; i < 4; i++) {
			printf("Sending packet %d to localhost port 21234\n", i);
			sprintf(buf, "This is packet %d", i);
			client->send(buf,strlen(buf));
			/* now receive an acknowledgement from the server */

			int recvlen = client->recv((char **)&buf,2048);
            if (recvlen >= 0) {
                    buf[recvlen] = 0;	/* expect a printable string - terminate it */
                    printf("received message: \"%s\"\n", buf);
            }
		}
	} catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
	}
}