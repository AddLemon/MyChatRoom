#include <iostream>
#include <thread>
#include "public.h"
#include "packet.h"

int main()
{
	cout << "Server start..." << endl;
	asio::io_context io;
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 5000));

	while (1) {
		/* Loop creating sockets and waiting for new clients to connect */
		SockPtr sockPtr(new tcp::socket(io));
		acceptor.accept(*sockPtr);
		cout << "Client " << sockPtr->remote_endpoint().address() << " connected!" << endl;

		/* receive the name of the client and record */
		string msg;
		string& msg_name = msg;
		if (!recvMsg(sockPtr, msg)) continue;
		cliMap[sockPtr] = msg;	//record socket pointer with client name in the map
#ifdef _WIN32
		cout << "Client: " << U8toA(cliMap[sockPtr]) << " is online!" << endl;
#else
		cout << "Client: " << cliMap[sockPtr] << " is online!" << endl;
#endif

		/* Enter the thread after connecting */
		thread dealCliThread(dealClient, sockPtr);
		dealCliThread.detach();
	}

	acceptor.close();
	return 0;
}