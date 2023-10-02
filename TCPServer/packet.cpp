#include "packet.h"

bool Packet::RecvMsg(SockPtr sockPtr, string& msg)
{
	try {
		string msg_tmp(50, '\0');
		sockPtr->receive(asio::buffer(msg_tmp));
		msg.assign(msg_tmp);
		//TODO name check

	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return false;
	}

	return true;
}

bool Packet::SendMsg(SockPtr sockPtr, const string& msg)
{
	//TODO message checksum

	try {
		sockPtr->send(asio::buffer(msg));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	return true;
}
