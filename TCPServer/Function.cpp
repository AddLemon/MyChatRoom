#include "Function.h"
#include "json/json.h"

Function::Function()
{
	cout << "Function is ready!" << endl;
}

Function::~Function()
{
	this->allUsers.Save();
}

void Function::InitData()
{
	this->allUsers.Load();
}

void Function::LogIn(SockPtr sockPtr, const string& recvMsg)
{
	//parse msg
	Json::Value jMsg;
	Json::Reader reader;
	reader.parse(recvMsg.c_str(), jMsg);
	const string id = jMsg["id"].asString();
	const string pswd = jMsg["password"].asString();

	//create send msg
	Json::Value jMsg_send;
	Json::FastWriter writer;
	string sendMsg{};

	//check if alreay online
	User user{};
	Result result{};
	if (!olUsers.FindUser(sockPtr, user)) {
		//not online
		result = allUsers.CheckCorrect(id, pswd, user);	//check for logging in
	}
	//insert to olUsers map
	olUsers.Insert(sockPtr, user);

	//send signal to sender client
	jMsg_send["serverSignal"] = result;
	sendMsg = writer.write(jMsg_send);

	pkg.SendMsg(sockPtr, sendMsg, serverSignal);	//server reply	
}

void Function::LogOff(SockPtr sockPtr)
{
	//remove from olUsers map
	olUsers.Remove(sockPtr);

	//TODO tell other clients that someone is log off
}

void Function::Register(SockPtr sockPtr, const string& recvMsg)
{
	//parse msg
	Json::Value jMsg;
	Json::Reader reader;
	reader.parse(recvMsg.c_str(), jMsg);
	const string id = jMsg["id"].asString();
	const string name = jMsg["name"].asString();
	const string pswd = jMsg["password"].asString();

	//create send msg
	Json::Value jMsg_send;
	Json::FastWriter writer;
	string sendMsg{};

	//check if already online
	Result result{};
	if (!olUsers.FindUser(sockPtr)) {
		//not online
		result = allUsers.CheckExit(id, name, pswd);	//check if id already exit
	}
	jMsg_send["serverSignal"] = result;
	sendMsg = writer.write(jMsg_send);

	pkg.SendMsg(sockPtr, sendMsg, serverSignal);	//server reply		
}

void Function::Settings(SockPtr sockPtr, const string& recvMsg)
{
	//parse msg
	Json::Value jMsg;
	Json::Reader reader;
	reader.parse(recvMsg.c_str(), jMsg);
	const string id = jMsg["id"].asString();
	const string name = jMsg["name"].asString();
	const string pswd = jMsg["password"].asString();
	User user = { id, name, pswd };

	//create send msg
	Json::Value jMsg_send;
	Json::FastWriter writer;
	string sendMsg{};

	//check if sender alreay online
	Result result{};
	if (olUsers.FindUser(sockPtr)) {
		//already online, renew user info in allUsers and olUsers
		allUsers.Modify(user);
		olUsers.RenewInfo(sockPtr, user);
		result = success;
	}

	//send signal to sender client
	result = notOnline;
	jMsg_send["serverSignal"] = result;
	sendMsg = writer.write(jMsg_send);

	pkg.SendMsg(sockPtr, sendMsg, serverSignal);	//server reply		
}

void Function::PrivateChat(SockPtr sockPtr, const string& recvMsg, const string& receiverID)
{
	//create send msg
	Json::Value jMsg_send;
	Json::FastWriter writer;
	const string sendMsg = recvMsg;
	string sendMsg_err{};

	//check if sender is already online
	User user{};
	if (olUsers.FindUser(sockPtr, user) != Result::success) {
		//sender not online
		jMsg_send["serverSignal"] = notOnline;
		sendMsg_err = writer.write(jMsg_send);

		pkg.SendMsg(sockPtr, sendMsg_err, serverSignal);	//server reply	
		return;
	}

	//check if receiverID is valid
	if (receiverID == "") {
		jMsg_send["serverSignal"] = receiverNotExit;
		sendMsg_err = writer.write(jMsg_send);

		pkg.SendMsg(sockPtr, sendMsg_err, serverSignal);	//server reply	
		return;
	}
	//get receiver`s socket
	SockPtr recvSockPtr;
	olUsers.FindSocket(receiverID, recvSockPtr);

	//check if receiver is also already online
	if (olUsers.FindUser(recvSockPtr)) {
		//receiver already online
		pkg.SendMsg(recvSockPtr, sendMsg, chat, user.id);	//Server forwards message	
	}
	//receiver not online
	jMsg_send["serverSignal"] = receiverNotOnline;
	sendMsg_err = writer.write(jMsg_send);

	pkg.SendMsg(sockPtr, sendMsg_err, serverSignal);	//server reply	
}

void Function::GetInstruction()
{
	//connected, now have socket
	asio::io_context io;
	SockPtr sockPtr(new tcp::socket(io));
	MsgType type{};
	string recvMsg{};
	string receiverID{};

	Packet pkg;
	pkg.RecvMsg(sockPtr, recvMsg, type, receiverID);	//receive msg

	switch (type) {
	case logIn: 
		LogIn(sockPtr, recvMsg);
		break;

	case newAccount: 
		Register(sockPtr, recvMsg);
		break;

	case chat: 
		PrivateChat(sockPtr, recvMsg, receiverID);
		break;
	
	case settings: 
		Settings(sockPtr, recvMsg);
		break;

	default:
		//err
	}
}
