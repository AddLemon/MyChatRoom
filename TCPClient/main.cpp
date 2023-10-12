#include <iostream>
#include <map>
#include <thread>
#include "public.h"
#include "CliFunction.h"
using namespace asio;

int main()
{
	io_context io;
	SockPtr sockPtr(new tcp::socket(io));

	string ipAddr, msg_name;

	// cout << "Please enter your name:";
	// getline(cin, msg_name);

	//cout << "Please enter the server`s IP address:";
	//cin >> ipAddr;

	CliFunction func;
	

	try {
		sockPtr->connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 5000));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	func.Init(sockPtr);
	func.InitialInterface();

	return 0;
}