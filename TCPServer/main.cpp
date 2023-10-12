#include <iostream>
#include <thread>
#include "public.h"
#include "packet.h"
#include "Function.h"

int main()
{
	cout << "Server start..." << endl;
	asio::io_context io;
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 5000));

	Function func;

	while (1) {
		/* Loop creating sockets and waiting for new clients to connect */
		SockPtr sockPtr(new tcp::socket(io));
		acceptor.accept(*sockPtr);
		cout << "Client " << sockPtr->remote_endpoint().address() << " connected!" << endl;

		

		/* Enter the thread after connecting */
		thread dealCliThread(&Function::DealClient, &func, sockPtr);
		dealCliThread.detach();
	}

	acceptor.close();
	return 0;
}