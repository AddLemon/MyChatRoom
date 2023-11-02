#include "Packet_Serv.h"

bool Packet_Serv::RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type, string* receiverID)
{
	//receive data and resize the buffer
	string sPacket(PACKET_CAPACITY, ' ');	
	size_t size;
	try {
		size = sockPtr->receive(asio::buffer(sPacket)); 
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return false;
	}
	sPacket.resize(size);

	//parse string to Json
	Json::Value jPacket;
	Json::Reader reader;
	if (!reader.parse(sPacket.data(), jPacket)) {
		cout << "Fail: Failed to parse data." << endl;		//test ********************
		return false;
	}

	Json::StyledWriter swriter;	//test ********************
	cout << "Receive:" << endl;	//test
	cout << swriter.write(jPacket) << endl;	//test
	cout << endl;	//test

	MsgType msgType = static_cast<MsgType>(jPacket["header"]["type"].asInt());
	size_t len = static_cast<size_t>(jPacket["header"]["length"].asInt());
	string id = jPacket["header"]["ID"].asString();			// userID or groupID for receiving
	string message = jPacket["message"].asString();
	char checkSum = static_cast<char>(jPacket["checkSum"].asInt());		//type conversion

	// check message checksum
	Json::Value jTest;
	Json::FastWriter writer;
	jTest["header"] = checkSum;
	string sTest = writer.write(jTest);

	char sum = 0;
	for (auto i : sPacket) {
		sum += i;
	}
	for (auto i : sTest) {
		sum -= i;
	}

	if (checkSum != sum) {
		cout << "Fail: Checksum is inconsistent." << endl;	//test ********************
		return false;
	}

	//Assign a value to the input containers
	msg = message;
	type = msgType;
	if (receiverID != nullptr) {
		*receiverID = id;
	}

	return true;
}

bool Packet_Serv::SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type, string* senderID)
{
	//write json
	Json::Value jPacket;
	Json::Value jPacket_header;
	Json::FastWriter writer;
	jPacket_header["type"] = type;
	jPacket_header["length"] = msg.size();
	jPacket_header["ID"] = senderID;
	jPacket["header"] = jPacket_header;
	jPacket["message"] = msg.data();
	
	// message checksum
	string sTest = writer.write(jPacket);	//for calculating checksum
	char checkSum = 0;
	for (auto i : sTest) {
		checkSum += i;
	}
	jPacket["checkSum"] = checkSum;	

	//json to string
	string sPacket = writer.write(jPacket);

	Json::StyledWriter swriter;		//test************************************
	cout << "send:" << endl;		//test
	cout << swriter.write(jPacket) << endl;	//test
	cout << endl;		//test

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

//int Packet_Serv::parseToJson(string& str, Json::Value& root, string* errMsg)
//{
//	Json::Reader reader;
//	Json::Value value;
//	if (!reader.parse(str.data(), value)) {
//		*errMsg = "Fail: Failed to parse data.";
//		return 0;
//	}
//	root = value;
//	return 1;
//}
//
//int Packet_Serv::writeToString(Json::Value& root, string& str, string* errMsg)
//{
//	Json::FastWriter writer;
//	str = writer.write(root);
//	return 0;
//}
