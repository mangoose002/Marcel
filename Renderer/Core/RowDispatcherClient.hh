#ifndef __ROW_DISPATCHER_CLIENT__
#define __ROW_DISPATCHER_CLIENT__

#include "Core/RowDispatcher.hh"
#include "Utils/Network/ClientSocket.hh"

namespace Marcel{

class RowDispatcherClient: public RowDispatcher{
	public:
		RowDispatcherClient(string host, int port, Image * aImage,int blocksize=1, int direction = DISPATCHER_DSC);
		~RowDispatcherClient();
		int nextRow();
	private:
		ClientSocket   *socket;
};
}

#endif