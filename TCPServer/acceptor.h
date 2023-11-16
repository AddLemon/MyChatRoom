#pragma once
#include "boost/asio.hpp"
#include "socket.h"

using namespace std;
using namespace boost;
using asio::ip::tcp;

class Acceptor
{
private:
	asio::io_context io;
	tcp::acceptor acceptor;
public:
	Acceptor() : acceptor(io, tcp::endpoint(tcp::v4(), 5000)) {};
	~Acceptor();

	Socket* createSocket();

	void accept(Socket* sockPtr);
	void close();
};

//asio::io_context Socket_Serv::io;
//tcp::acceptor Socket_Serv::acceptor;