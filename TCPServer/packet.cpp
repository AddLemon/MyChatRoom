#include "packet.h"

bool Packet::RecvMsg(const SockPtr sockP, string& msg, MsgType& type)
{
	Header header;
	string msg_tmp(50, '\0');
	try {
		sockP->receive(asio::buffer(header));
		sockP->receive(asio::buffer(msg_tmp)); // TODO try msg.data();

		// check message checksum
		char sum = 0;
		for (auto i : msg_tmp) {
			sum += i;
		}
		if (header.checkSum != sum) return false;
		msg.resize(header.len);
		msg.assign(msg_tmp);
		type = header.type;
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return false;
	}

	return true;
}

bool Packet::SendMsg(const SockPtr sockP, const string& msg, const MsgType type)
{
	Header header;
	// message checksum
	for (auto i : msg) {
		header.checkSum += i;
	}
	header.type = type;
	header.len = msg.size();

	try {
		sockP->send(asio::buffer(header));
		sockP->send(asio::buffer(msg));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	return true;
}
