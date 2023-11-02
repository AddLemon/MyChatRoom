#pragma once
#include "boost/asio.hpp"
using namespace std;
using namespace boost;
using asio::ip::tcp;

typedef tcp::socket* SockPtr;

class Socket_Serv
{
private:
	asio::io_context io;
	tcp::acceptor acceptor;
public:
	Socket_Serv() : acceptor(io, tcp::endpoint(tcp::v4(), 5000)) {};
	~Socket_Serv();

	SockPtr createSocket();

	void accept(SockPtr sockPtr);
};

//asio::io_context Socket_Serv::io;
//tcp::acceptor Socket_Serv::acceptor;