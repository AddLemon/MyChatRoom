#include "cliFunction.h"

CliFunction::CliFunction()
{
	cout << "Client start..." << endl;
}

CliFunction::~CliFunction()
{
	cout << "Client will be closed in 3 seconds." << endl;
	Sleep(3000);
}

void CliFunction::Init(SockPtr sockPtr)
{
	this->serverSock = sockPtr;
}

bool CliFunction::LogIn()
{
	//char id_tmp[USER_ID_LENGTH];
	string id, password;
	cout << "Please enter your id: ";
	cin >> id;
	//TODO check length
	cout << "Please enter your password: ";
	cin >> password;

	//create json and write as string to send 
	Json::Value jMsg_send;
	Json::FastWriter writer;
	jMsg_send["id"] = id.data();
	jMsg_send["password"] = password.data();
	string sendMsg = writer.write(jMsg_send);

	//send to server for check in
	if (pkg.SendMsg(serverSock, sendMsg, logIn)) {
		cout << "Send successfully." << endl;		//success window/icon
	}
	else {
		cout << "Failed to send." << endl;		//err window/icon
	}

	//receive server`s reply
	//TODO Encapsulation
	string replyMsg{};
	MsgType msgType{};
	Json::Value jMsg_reply;
	Json::Reader reader;
	Result serverSignal{};
	pkg.RecvMsg(serverSock, replyMsg, msgType);
	
	//parse reply message
	if (msgType == MsgType::serverSignal) {
		reader.parse(replyMsg.data(), jMsg_reply);
	}
	else {
		cout << "Wrong reply." << endl;		//err window/icon
	}
	//output server feedback
	serverSignal = static_cast<Result>(jMsg_reply["serverSignal"].asInt());
	cout << "		** Server: " << DescribeServSignal(serverSignal) << " **" << endl;

	if (serverSignal == Result::success) {
		return true;
	}
	else return false;
}

void CliFunction::LogOff()
{
	serverSock->close();
}

bool CliFunction::Register()
{
	//get info
	string id, name, password, password_tmp;
	cout << "Please enter your id: ";
	cin >> id;
	//TODO check length
	cout << "Please enter your password: ";
	cin >> password;
	cout << "Please enter your password again: ";
	cin >> password_tmp;
	if (password_tmp != password) {
		cout << "Password is not match, please try again." << endl;
		return false;
	}
	cout << "Please enter your name: ";
	cin >> name;

	//write json and send message
	Json::Value jMsg_send;
	Json::FastWriter writer;
	//writer.omitEndingLineFeed();	//omit FastWriter to add '\n' automatically
	jMsg_send["id"] = id;
	jMsg_send["name"] = name;
	jMsg_send["password"] = password;
	string sMsg_send = writer.write(jMsg_send);

	pkg.SendMsg(serverSock, sMsg_send, MsgType::newAccount);

	//receive reply
	string sMsg_reply{};
	MsgType msgType{};
	Result serverSignal{};
	Json::Value jMsg_reply;
	Json::Reader reader;
	pkg.RecvMsg(serverSock, sMsg_reply, msgType);

	//parse reply message
	//TODO Encapsulation *************************
	if (msgType == MsgType::serverSignal) {
		reader.parse(sMsg_reply.data(), jMsg_reply);
	}
	else {
		cout << "Wrong reply." << endl;		//err window/icon
	}
	//output server feedback
	serverSignal = static_cast<Result>(jMsg_reply["serverSignal"].asInt());
	cout << "		** Server:" << DescribeServSignal(serverSignal) << " **" << endl;

	if (serverSignal == Result::success) {
		return true;
	}
	else return false;

}

bool CliFunction::Settings()
{
	return true;
}

bool CliFunction::PrivateChat()
{
	return true;
}

void CliFunction::InitialInterface()
{
	//initial interface
	while (1) {
		int a;
		cout << "Choice the function you need: 1. Log in		2. Register" << endl;
		cout << "The function number is: ";
		cin >> a;
		switch (a) {
		case 1: {
			if (LogIn()) {
				MainInterface();
			}
			break;
		}
		case 2: {
			if (Register()) {
				InitialInterface();
			}
			break;
		}
		default: {
			cout << "Invalid number! Try again." << endl;
		}
		}
	}
}

void CliFunction::MainInterface()
{
	while (1) {
		int a;
		cout << "Choice the function you need: 1. PrivateChat		2. Settings		3. Log off" << endl;
		cout << "The function number is: ";
		cin >> a;

		switch (a) {
		case 1: {
			if (PrivateChat()) {
				cout << "privateChat" << endl;
				//TODO
			}
			else break;
		}
		case 2: {
			if (Settings()) {
				cout << "Settings" << endl;
				//TODO
			}
			else break;
		}
		case 3: {
			LogOff();
		}
		default: {
			cout << "Invalid number! Try again." << endl;
		}
		}
	}
}

