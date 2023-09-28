#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include "public.h"
using namespace asio;

int main()
{
	io_context io;
	SockPtr sockPtr(new tcp::socket(io));

	string ipAddr, userName;
	vector<char> msg_name;
	cout << "Please enter the server`s IP address:";
	cin >> ipAddr;
	cout << "Please enter your name:";
	cin >> userName;

	msg_name.resize(userName.size());
	msg_name.assign(userName.begin(), userName.end());

	sendMsg(sockPtr, msg_name);

	while (1) {
		char a[50];
		cout << "you: ";
		cin.get(a, 50);
		vector<char> msg(a, a + strlen(a));
		sendMsg(sockPtr, msg);
	}


	return 0;
}