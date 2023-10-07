#include "packet.h"

bool Packet::RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type, string& receiverID)
{
	string sPacket(50, '/0');	//try ' '
	try {
		sockPtr->receive(asio::buffer(sPacket)); // TODO try msg.data();
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return false;
	}

	//parse string
	Json::Reader reader;
	Json::Value jPacket;
	if (!reader.parse(sPacket.c_str(), jPacket)) {
		cout << "Fail: Failed to parse data." << endl;
		return false;
	}

	char checkSum = static_cast<char>(jPacket["header"]["checkSum"].asInt());		//type conversion
	MsgType msgType = static_cast<MsgType>(jPacket["header"]["type"].asInt());
	size_t len = static_cast<size_t>(jPacket["header"]["length"].asInt());
	string id = jPacket["header"]["senderID"].asString();
	string message = jPacket["message"].asString();

	// check message checksum
	char sum = 0;
	for (auto i : sPacket) {
		sum += i;
	}
	if (checkSum != sum) {
		cout << "Fail: Checksum is inconsistent." << endl;
		return false;
	}

	//Assign a value to the input containers
	msg = message;
	type = msgType;
	receiverID = id;

	return true;
}

bool Packet::SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type, const string& senderID)
{
	// message checksum
	char checkSum = 0;
	for (auto i : msg) {
		checkSum += i;
	}

	//write json
	Json::Value jPacket;
	Json::Value jPacket_first;
	jPacket_first["checkSum"] = checkSum;
	jPacket_first["type"] = type;
	jPacket_first["length"] = msg.size();
	jPacket_first["senderID"] = senderID;
	jPacket["header"] = jPacket_first;
	jPacket["message"] = msg.c_str();

	//json to string
	Json::FastWriter writer;
	string sPacket = writer.write(jPacket);

	//send packet
	try {
		sockPtr->send(asio::buffer(sPacket));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	return true;
}

bool Packet::SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type)
{
	// message checksum
	char checkSum = 0;
	for (auto i : msg) {
		checkSum += i;
	}

	//write json
	Json::Value jPacket;
	Json::Value jPacket_first;
	jPacket_first["checkSum"] = checkSum;
	jPacket_first["type"] = type;
	jPacket_first["length"] = msg.size();
	jPacket_first["senderID"] = "";
	jPacket["header"] = jPacket_first;
	jPacket["message"] = msg.c_str();

	//json to string
	Json::FastWriter writer;
	string sPacket = writer.write(jPacket);

	//send packet
	try {
		sockPtr->send(asio::buffer(sPacket));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}

	return true;
}
