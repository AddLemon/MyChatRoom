#include "Socket_Serv.h"

//Socket_Serv::Socket_Serv():acceptor(io, tcp::endpoint(tcp::v4(), 5000))
//{
//	acceptor = tcp::acceptor(io, tcp::endpoint(tcp::v4(), 5000));
//}

Socket_Serv::~Socket_Serv()
{
	acceptor.close();
}

SockPtr Socket_Serv::createSocket()
{
	SockPtr sockPtr(new tcp::socket(io));
	return sockPtr;
}

void Socket_Serv::accept(SockPtr sockPtr)
{
	acceptor.accept(*sockPtr);
}
