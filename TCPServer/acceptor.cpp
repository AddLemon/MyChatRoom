#include "acceptor.h"

Acceptor::~Acceptor()
{
	acceptor.close();
}

Socket* Acceptor::createSocket()
{
	//Socket sockPtr = Socket(new tcp::socket(io));
	Socket* sockPtr = new Socket(&io);
	return sockPtr;
}

void Acceptor::accept(Socket* sockPtr)
{
	acceptor.accept(*sockPtr->get());
}

void Acceptor::close()
{
	acceptor.close();
}
