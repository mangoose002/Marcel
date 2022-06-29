#include "Utils/Network/SocketException.hh"
#include "Utils/Utils.hh"
#include "Core/Scene.hh"

#include "Core/Image.hh"
#include "Core/ImageClient.hh"

#include "Core/RowDispatcher.hh"
#include "Core/RowDispatcherClient.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <limits.h> /* PATH_MAX */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <zlib.h>

using namespace std;

namespace Marcel{

	Utils::Utils() {};
	Utils::~Utils() {};


	int Utils::CompressFile(string input, string output){
		char buffer[1024];
		int size;

		gzFile compressed   = gzopen(output.c_str(), "wb9");
		FILE   *uncompressed = fopen(input.c_str(), "rb");

		while (!feof(uncompressed))
		{
			size = fread(buffer, 1, 1024, uncompressed);
			gzwrite(compressed, buffer, size);
		}

		fclose(uncompressed);
		gzclose(compressed);

		return Utils::filesize(output);
	}

	int Utils::UncompressFile(string input, string output){
		char buffer[1024];
		int size;

		gzFile compressed   = gzopen(input.c_str(), "rb");
		FILE   *uncompressed = fopen(output.c_str(), "wb");

		while (!gzeof(compressed))
		{
			size = gzread(compressed, buffer, 1024);
			fwrite(buffer, 1, size, uncompressed);
		}

		fclose(uncompressed);
		gzclose(compressed);

		return Utils::filesize(output);
	}

	SceneContext* Utils::ParseCommandLine(int argc, char* argv[], int mode){

		int c;
		//int threadnumber;
		//int antialias;
		//

		SceneContext *context  = new SceneContext;

		context->OutputFileName     = "Marcel.jpg";
		context->InputFileName      = "Marcel.3ds";
		context->XResolution        = 720;
		context->YResolution        = 480;
		context->isProgressive      = false;
		context->isAliased          = false;
		context->isSoftshadowed     = false;
		context->isRaycasting       = false;
		context->MAX_LEVEL          = 5;
		context->MAX_OCTREE_LEVEL   = 8;
		context->MAX_QUADTREE_LEVEL = 8;
		context->Blocksize          = 8;
		context->Threshold          = 4;

		context->hostname           = "localhost";
		context->port               = 8888;

		context->ThreadNumber = thread::hardware_concurrency();
		if (context->ThreadNumber < 1)
			context->ThreadNumber = 1;

		while (1)
		{
			static struct option long_options[] =
			{
				{"progressive",  no_argument,       0, 'P'},
				{"softshadow",   no_argument,       0, 'S'},
				{"mono",         no_argument,       0, '1'},
				{"dual",         no_argument,       0, '2'},
				{"aliased",      no_argument,       0, 'a'},
				{"help",         no_argument,       0, 'H'},
				{"raycasting",   no_argument,       0, 'R'},
				{"threshold",    required_argument, 0, 't'},
				{"blocksize",    required_argument, 0, 'b'},
				{"quadtree",     required_argument, 0, 'O'},
				{"octree",       required_argument, 0, 'Q'},
				{"output",       required_argument, 0, 'o'},
				{"input",        required_argument, 0, 'i'},
				{"width",        required_argument, 0, 'w'},
				{"height",       required_argument, 0, 'h'},
				{"host",         required_argument, 0, 'h'},
				{"port",         required_argument, 0, 'p'}  //For server/client mode
			};
			/* getopt_long stores the option index here. */
			int option_index = 0;

			c = getopt_long (argc, argv, "t:b:o:i:w:h:p:O:Q:RPS12aH",
			                 long_options, &option_index);

			/* Detect the end of the options. */
			if (c == -1)
				break;

			switch (c){
			case 'P':
				if (mode == _CLIENT_MODE_)
					cout << "Progressive mode cannot be set in client mode" << endl;
				else
					context->isProgressive = true;
				break;

			case 'S':
				if (mode == _CLIENT_MODE_)
					cout << "Softshadow mode cannot be set in client mode" << endl;
				else
					context->isSoftshadowed = true;
				break;

			case 'a':
				if (mode == _CLIENT_MODE_)
					cout << "Antialias mode cannot be set in client mode" << endl;
				else
					context->isAliased = 1;
				break;
			case 'R':
				if (mode == _CLIENT_MODE_)
					cout << "Raycasting mode cannot be set in client mode" << endl;
				else
					context->isRaycasting = 1;
				break;

			case '1':
				if (mode == _CLIENT_MODE_)
					cout << "MonoThread mode cannot be set in client mode" << endl;
				else
					context->ThreadNumber = 1;
				break;

			case '2':
				if (mode == _CLIENT_MODE_)
					cout << "DualThread mode cannot be set in client mode" << endl;
				else
					context->ThreadNumber = 2;
				break;
			case 'H':
				cout << "Help" << endl;
				break;

			case 'Q':
				context->MAX_QUADTREE_LEVEL = atoi(optarg);
				break;
			case 'O':
				context->MAX_OCTREE_LEVEL = atoi(optarg);
				break;

			case 'p':
				if (mode == _SERVER_MODE_ || mode == _CLIENT_MODE_)
					context->port = atoi(optarg);
				else
					cout << "Port cannot be set in local mode. Discarding" << endl;
				break;

			case 't':
				if (context->isProgressive == false)
					cout << "Threshold can only be set in progressive mode" << endl;
				else if (mode == _CLIENT_MODE_)
					cout << "Threshold cannot be set in client mode" << endl;
				else
					context->Threshold = atoi(optarg);
				break;

			case 'b':
				if (context->isProgressive == false)
					cout << "Blocksize can only be set in progressive mode" << endl;
				else if (mode == _CLIENT_MODE_)
					cout << "Blocksize cannot be set in client mode. Discarding" << endl;
				else
					context->Blocksize = atoi(optarg);
				break;

			case 'o':
				if (mode == _CLIENT_MODE_)
					cout << "Output filename cannot be set in client mode. Discarding" << endl;
				else
					context->OutputFileName = optarg;
				break;

			case 'i':
				if (mode == _CLIENT_MODE_)
					cout << "Input filename cannot be set in client mode. Discarding" << endl;
				else
					context->InputFileName = optarg;
				break;

			case 'w':
				if (mode == _CLIENT_MODE_)
					cout << "Width cannot be set in client mode. Discarding" << endl;
				else
					context->XResolution = atoi(optarg);
				break;

			case 'h':
				if (mode == _CLIENT_MODE_) {
					context->hostname = optarg;
				} else {
					context->YResolution = atoi(optarg);
				}
				break;

			default:
				abort ();
			}
		}

		if (mode == _LOCAL_MODE_) {
			//context->File       = new Image(context->OutputFileName.c_str(), context->XResolution, context->YResolution);
			//context->Dispatcher = new RowDispatcher(context->File);

			cout << "--------------------- Parameters ----------------------------" << endl;
			cout << "Input filename:     " << context->InputFileName << endl;
			cout << "Output filename:    " << context->OutputFileName << endl;
			cout << "Output dimension:   " << context->XResolution << "x" << context->YResolution << endl;
			cout << "Antialias:          " << ((context->isAliased) ? "Yes" : "No") << endl;
			cout << "Soft shadows:       " << ((context->isSoftshadowed) ? "Yes" : "No") << endl;
			cout << "Raycasting:         " << ((context->isRaycasting) ? "Yes" : "No") << endl;
			cout << "-------------------------------------------------------------" << endl;
			cout << "Max depth level:    " << context->MAX_LEVEL << endl;
			cout << "Max octree level:   " << context->MAX_OCTREE_LEVEL << endl;
			cout << "Max quadtree level: " << context->MAX_QUADTREE_LEVEL << endl;
			cout << "-------------------------------------------------------------" << endl;
			if (context->isProgressive) {
				cout << "Progressive:        True" << endl;
				cout << "Threshold:          " << context->Threshold << endl;
				cout << "Blocksize:          " << context->Blocksize << endl;
			} else {
				cout << "Progressive:        No" << endl;
			}
		}

		return context;
	}

	int Utils::filesize(string filename) {
		FILE *fp = fopen(filename.c_str(), "rb");
		if (fp != NULL) {
			int prev = ftell(fp);
			fseek(fp, 0L, SEEK_END);
			int sz = ftell(fp);
			fseek(fp, prev, SEEK_SET); //go back to where we were
			fclose(fp);

			return sz;
		}
		return -1;
	}

	string Utils::filepath(string filename) {
		cout << filename << endl;
		char *res = realpath(filename.c_str(), NULL);


		string Fname = filename.substr(filename.find_last_of("/\\") + 1);
		string Realname = res;

		return Realname.substr(0, Realname.size() - Fname.size() - 1);
	}

}
