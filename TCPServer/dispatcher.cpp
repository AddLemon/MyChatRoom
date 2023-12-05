#include "dispatcher.h"

Dispatcher::Dispatcher(ThreadPool* tp)
{
	m_threadPool = tp;
}

void Dispatcher::dispatch(Json::Value pkt)
{
	int requestID = pkt["header"]["requestID"].asInt();
	ClientMsgType type = static_cast<ClientMsgType>(pkt["header"]["type"].asInt());
	string senderID = pkt["header"]["senderID"].asString();
	string userID;
	string userName;
	string password;
	int groupID;
	string groupName;
	string receiverID;
	string content;
	string timestamp;

	switch (type)
	{
	case ClientMsgType::REGISTER_UP:
	{
		userID = pkt["message"]["userID"].asString();
		userName = pkt["message"]["userName"].asString();
		password = pkt["message"]["password"].asString();
		RegisterCommand* cmd = new RegisterCommand(requestID, senderID, userID, userName, password);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::LOG_IN:
	{
		userID = pkt["message"]["userID"].asString();
		password = pkt["message"]["password"].asString();
		LogInCommand* cmd = new LogInCommand(requestID, senderID, userID, password);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::MODIFY_USER_INFO:
	{
		userName = pkt["message"]["userName"].asString();
		password = pkt["message"]["password"].asString();
		MdfyUsrInfoCommand* cmd = new MdfyUsrInfoCommand(requestID, senderID, userName, password);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::FIND_USER:
	{
		userID = pkt["message"]["userID"].asString();
		FindUsrCommand* cmd = new FindUsrCommand(requestID, senderID, userID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::ADD_FRIEND:
	{
		userID = pkt["message"]["userID"].asString();
		AddFrndCommand* cmd = new AddFrndCommand(requestID, senderID, userID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::REMOVE_FRIEND:
	{
		userID = pkt["message"]["userID"].asString();
		RmvFrndCommand* cmd = new RmvFrndCommand(requestID, senderID, userID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::FIND_GROUP:
	{
		groupID = pkt["message"]["groupID"].asInt();
		FindGrpCommand* cmd = new FindGrpCommand(requestID, senderID, groupID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::ADD_GROUP:
	{
		groupID = pkt["message"]["groupID"].asInt();
		AddGrpCommand* cmd = new AddGrpCommand(requestID, senderID, groupID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::REMOVE_GROUP:
	{
		groupID = pkt["message"]["groupID"].asInt();
		RmvGrpCommand* cmd = new RmvGrpCommand(requestID, senderID, groupID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::GET_GROUP_MEMBERS:
	{
		groupID = pkt["message"]["groupID"].asInt();
		GetGrpMbrCommand* cmd = new GetGrpMbrCommand(requestID, senderID, groupID);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::CREATE_GROUP:
	{
		groupName = pkt["message"]["groupName"].asString();
		CrtGrpCommand* cmd = new CrtGrpCommand(requestID, senderID, groupName);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::MODIFY_GROUP_INFO:
	{
		groupID = pkt["message"]["groupID"].asInt();
		groupName = pkt["message"]["groupName"].asString();
		MdfyGrpInfoCommand* cmd = new MdfyGrpInfoCommand(requestID, senderID, groupID, groupName);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::PRIVATE_CHAT:
	{
		receiverID = pkt["header"]["receiverID"].asString();
		content = pkt["message"]["content"].asString();
		timestamp = pkt["message"]["timestamp"].asString();
		PvChatCommand* cmd = new PvChatCommand(requestID, senderID, receiverID, content, timestamp);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::GROUP_CHAT:
	{
		groupID = pkt["header"]["groupID"].asInt();
		content = pkt["message"]["content"].asString();
		timestamp = pkt["message"]["timestamp"].asString();
		GpChatCommand* cmd = new GpChatCommand(requestID, senderID, groupID, content, timestamp);
		m_threadPool->addTask(cmd);
		break;
	}
	case ClientMsgType::LOG_OFF:
	{
		LogOffCommand* cmd = new LogOffCommand(requestID, senderID);
		m_threadPool->addTask(cmd);
		break;
	}
	default:
		cout << "Wrong client message type.." << endl;
	}
}
