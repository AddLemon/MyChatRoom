#include "public.h"

bool recvMsg(SockPtr sockPtr, vector<char>& msg)
{
	try {
		vector<char> msg_tmp;
		sockPtr->receive(msg_tmp);
		//TODO name check

		msg = msg_tmp;
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return false;
	}

	return true;
}


bool sendMsg(SockPtr sockPtr, vector<char>& msg)
{
	//TODO message checksum

	try {
		sockPtr->send(msg);
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	return true;
}
