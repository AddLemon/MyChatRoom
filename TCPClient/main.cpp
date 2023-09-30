#include <iostream>
#include <map>
#include <thread>
#include "public.h"
using namespace asio;

int main()
{
	io_context io;
	SockPtr sockPtr(new tcp::socket(io));

	string ipAddr, msg_name;

	cout << "Please enter your name:";
	getline(cin, msg_name);

	//cout << "Please enter the server`s IP address:";
	//cin >> ipAddr;

	try {
		sockPtr->connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 5000));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	sendMsg(sockPtr, msg_name);

	while (1) {
		string a, remsg;
		cout << "you: ";
		getline(cin, a);

		sendMsg(sockPtr, a);
		recvMsg(sockPtr, remsg);
		cout << "server: " << remsg << endl;
	}

	return 0;
}