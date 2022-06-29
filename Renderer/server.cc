
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Utils/Utils.hh"

#include "Utils/Network/define.hh"

#include "Utils/Network/Socket.hh"
double Marcel::Socket::time_spent_recv = 0;
double Marcel::Socket::time_spent_send = 0;

#include "Utils/Network/SocketException.hh"
#include "Utils/Network/ServerSocket.hh"

#include "Core/Object.hh"
#include "Core/Image.hh"
#include "Core/RowDispatcher.hh"
uint64_t Marcel::Objet::SuccessfulIntersectionNumber = 0;
uint64_t Marcel::Objet::IntersectionNumber = 0;

using namespace std;
//using namespace tthread;
using namespace Marcel;

int locked = true;
mutex ImageMutex;
int RemainingLines = 0;

struct ThreadContext {
	ServerSocket   *sSocket;
	RowDispatcher  *Dispatcher;
	Image          *File;

	string         InputFilename;
	string         OutputFilename;

	int            isProgressive;
	int            Threshold;
	int            Blocksize;

	int            isAliased;

	int            XResolution;
	int            YResolution;

	int            ThreadNumber;
};

// Condition variable
mutex gMutex;
condition_variable gCond;

void RowDispatcherServer(void *aArg) {
	int type;
	int line;

	ServerSocket   socket     = *((ThreadContext *)aArg)->sSocket;
	RowDispatcher *Dispatcher =  ((ThreadContext *)aArg)->Dispatcher;

	std::cout << "RowDispatcherClient is connected" << endl;

	try {
		socket.send("", _ROWDISPATCHER_CLIENT_ACK_);
		while (true) {
			type = socket.recv(4);

			{
				lock_guard<mutex> guard(gMutex);
				if (type == _LINE_REQUEST_) {
					while (locked == true) {
						std::unique_lock<std::mutex> lk(gMutex);
						gCond.wait(lk);
					}
					line = Dispatcher->nextRow();

					socket.send("%w", _LINE_RESPONSE_, line);
				}
			}
		}
	}
	catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
	}
}

void ImageServer(void *aArg) {
	int line;
	int type;

	Image *File         =  ((ThreadContext *)aArg)->File;
	ServerSocket socket = *((ThreadContext *)aArg)->sSocket;

	std::cout << "ImageClient is connected" << endl;

	try {
		socket.send("%w%w%s", _IMAGE_CLIENT_ACK_, File->Width(), File->Height(), File->getOuputFilename().c_str());
		type = socket.recv(4);
		if (type != _IMAGE_CLIENT_SIZE_ACK_) {
			throw SocketException("Protocol error");
		}

		while (true) {

			type = socket.recv(4 + 2 + 3 * File->Width(),"%w", &line);
			if (type == _LINE_RESULT_ && line <= File->Height()) {
				for (int i = 0; i < File->Width(); i++) {
					File->setColor(socket.getHandler()->getColor(i), i, File->Height() - line - 1);
				}
				{
					lock_guard<mutex> guard(ImageMutex);
					RemainingLines++;
					cout << "Received lines: " << RemainingLines << endl;

					if (RemainingLines == (File->Height() -1)) {
						cout << "Receive: " << Socket::time_spent_recv << endl;
						cout << "Send:    " << Socket::time_spent_send << endl;

						File->saveImage();
						break;
					}
					socket.send("", _LINE_RESULT_ACK_);
				}
			}
		}
	}
	catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
	}
}

void ConfigServer(void *aArg){
	ServerSocket socket = *((ThreadContext *)aArg)->sSocket;

	std::cout << "ConfigClient is connected" << endl;
	try {
		socket.send("%w%w%w%w%w%w%w%i", _CONFIG_CLIENT_ACK_,
								((ThreadContext *)aArg)->XResolution,
								((ThreadContext *)aArg)->YResolution,
			                  	((ThreadContext *)aArg)->isAliased,
			                  	((ThreadContext *)aArg)->isProgressive,
			                  	((ThreadContext *)aArg)->Blocksize,
			                  	((ThreadContext *)aArg)->Threshold,
			                  	((ThreadContext *)aArg)->ThreadNumber,
			                  	Utils::filesize(((ThreadContext *)aArg)->InputFilename));

		socket.sendFile(((ThreadContext *)aArg)->InputFilename);
	}
	catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
		//File->saveImage();
	}
}

void LockingThread() {
	lock_guard<mutex> lock(gMutex);
	//int c = fgetc(stdin);
//	std::cout << c << std::endl;

	locked = false;
	gCond.notify_all();
}

int main ( int argc, char* argv[] )
{
	ThreadContext *context = new ThreadContext;
	context->isProgressive = 0;
	context->Threshold     = 4;
	context->Blocksize     = 8;
	context->isAliased     = 0;
	context->XResolution   = 360;
	context->YResolution   = 240;
	context->ThreadNumber  = -1;

	Image *File;
	int port = 8888;
	int c;

	context->OutputFilename = "Marcel.jpg";
	context->InputFilename  = "Marcel.3ds";

	while (1)
	{
		static struct option long_options[] =
		{
			{"progressive",  no_argument,       0, 'P'},
			{"mono",         no_argument,       0, '1'},
			{"dual",         no_argument,       0, '2'},
			{"output",       required_argument, 0, 'o'},
			{"input",        required_argument, 0, 'i'},
			{"port",         required_argument, 0, 'p'},
			{"width",        required_argument, 0, 'w'},
			{"height",       required_argument, 0, 'h'}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "i:o:p:w:h:P12",
		                 long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case '1':
			context->ThreadNumber = 1;
			break;

		case '2':
			context->ThreadNumber = 2;
			break;

		case 'o':
			context->OutputFilename = optarg;
			break;

		case 'i':
			context->InputFilename = optarg;
			break;

		case 'p':
			port = atoi(optarg);
			break;

		case 'w':
			context->XResolution = atoi(optarg);
			break;

		case 'P':
			context->isProgressive = true;
			break;

		case 'h':
			context->YResolution = atoi(optarg);
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort ();
		}
	}


	int type;
	cout << "/////////////////////////////////////////////////////" << endl;
	cout << "Input filename:  " << context->InputFilename << endl;
	cout << "Output filename: " << context->OutputFilename << endl;
	cout << "Image size:      " << context->XResolution << "x" << context->YResolution << endl;
	cout << "/////////////////////////////////////////////////////" << endl;

	//Preparing data for communication
	File = new Image(context->OutputFilename.c_str(), context->XResolution, context->YResolution);
	RowDispatcher *Dispatcher = new RowDispatcher(File);

	context->File = File;
	context->Dispatcher = Dispatcher;
	RemainingLines = 0;

	std::cout << "running....\n";
	ServerSocket* new_sock;


	thread *t = new thread(LockingThread);
	t->detach();

	try
	{
		// Create the socket
		ServerSocket server (port);

		while ( true )
		{
			new_sock = new ServerSocket();
			server.accept((Socket *)new_sock);
			context->sSocket = new_sock;

			type = new_sock->recv(4);
			switch (type) {
			case _ROWDISPATCHER_CLIENT_:
			{
				thread *t = new thread(RowDispatcherServer, (void *)context);
				t->detach();
			}
			break;
			case _IMAGE_CLIENT_:
			{
				thread *t = new thread(ImageServer, (void *)context);
				t->detach();
			}
			break;
			case _CONFIG_CLIENT_:
			{
				thread *t = new thread(ConfigServer, (void *)context);
				t->detach();
			}
			break;
			default:
				std::cout << "Not a know client type" << std::endl;
				delete new_sock;
			}
		}
	}
	catch ( SocketException &e ) {
		std::cout << e.description() << std::endl;
	}

	return 0;
}
