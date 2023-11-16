#pragma once
#include "Server.h"
class Command
{
public:
	Command(int requestID, string senderID);
	virtual ~Command() {};
	virtual void execute() = 0;
	virtual void send();
	virtual void addHeader();
	void addCheckSum(Json::Value& pkt);

protected:
	Server* m_server;
	int m_requestID;
	string m_senderID;
	ServerMsgType m_reply_type;
	Json::Value m_reply_pkt;
};

class RegisterCommand : public Command
{
public:
	RegisterCommand(int requestID, string senderID, string id, string name, string password)
		:Command(requestID, senderID), m_id(id), m_name(name), m_password(password) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_REGISTER_UP;
	};

	void execute() override;
private:
	string m_id;
	string m_name;
	string m_password;
};

class LogInCommand : public Command
{
public:
	LogInCommand(int requestID, string senderID, string id, string password)
		:Command(requestID, senderID), m_id(id), m_password(password) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_LOG_IN;
	};

	void execute() override;
private:
	string m_id;
	string m_password;
};

class MdfyUsrInfoCommand : public Command
{
public:
	MdfyUsrInfoCommand(int requestID, string senderID, string name, string password)
		:Command(requestID, senderID), m_name(name), m_password(password) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_MODIFY_USER_INFO;
	};

	void execute() override;
private:
	string m_name;
	string m_password;
};

class FindUsrCommand : public Command
{
public:
	FindUsrCommand(int requestID, string senderID, string id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_FIND_USER;
	};

	void execute() override;
private:
	string m_id;
};

class AddFrndCommand : public Command
{
public:
	AddFrndCommand(int requestID, string senderID, string id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_ADD_FRIEND;
		m_notice_type = ServerMsgType::SERVER_NOTICE_NEW_FRIEND;
	};

	void execute() override;
	void send() override;
	void addHeader() override;
private:
	string m_id;
	ServerMsgType m_notice_type;
	Json::Value m_notice_pkt;
};

class RmvFrndCommand : public Command
{
public:
	RmvFrndCommand(int requestID, string senderID, string id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_REMOVE_FRIEND;
		m_notice_type = ServerMsgType::SERVER_NOTICE_REMOVE_FRIEND;
	};

	void execute() override;
	void send() override;
	void addHeader() override;
private:
	string m_id;
	ServerMsgType m_notice_type;
	Json::Value m_notice_pkt;
};

class FindGrpCommand : public Command
{
public:
	FindGrpCommand(int requestID, string senderID, int id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_FIND_GROUP;
	};

	void execute() override;
private:
	int m_id;
};

class RmvGrpCommand : public Command
{
public:
	RmvGrpCommand(int requestID, string senderID, int id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_REMOVE_GROUP;
	};

	void execute() override;
private:
	int m_id;
};

class AddGrpCommand : public Command
{
public:
	AddGrpCommand(int requestID, string senderID, int id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_ADD_GROUP;
	};

	void execute() override;
private:
	int m_id;
};

class GetGrpMbrCommand : public Command
{
public:
	GetGrpMbrCommand(int requestID, string senderID, int id)
		:Command(requestID, senderID), m_id(id) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_GET_GROUP_MEMBERS;
	};

	void execute() override;
private:
	int m_id;
};

class CrtGrpCommand : public Command
{
public:
	CrtGrpCommand(int requestID, string senderID, string name)
		:Command(requestID, senderID), m_groupName(name) 
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_CREATE_GROUP;
	};

	void execute() override;
private:
	string m_groupName;
};

class MdfyGrpInfoCommand : public Command
{
public:
	MdfyGrpInfoCommand(int requestID, string senderID, int groupID, string name)
		:Command(requestID, senderID), m_groupID(groupID), m_groupName(name)
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_MODIFY_GROUP_INFO;
	};

	void execute() override;
private:
	int m_groupID;
	string m_groupName;
};

class GpChatCommand : public Command
{
public:
	GpChatCommand(int requestID, string senderID, int groupID, string content, string timestamp)
		:Command(requestID, senderID), m_groupID(groupID), m_content(content), m_timestamp(timestamp)
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_GROUP_CHAT;
		m_forward_type = ServerMsgType::SERVER_FORWARD_GROUP_CHAT;
	};

	void execute() override;
	void send() override;
	void addHeader() override;
private:
	queue<string> m_online_members;
	int m_groupID;
	string m_content;
	string m_timestamp;
	ServerMsgType m_forward_type;
	Json::Value m_forward_pkt;
};

class PvChatCommand : public Command
{
public:
	PvChatCommand(int requestID, string senderID, string receiverID, string content, string timestamp)
		:Command(requestID, senderID), m_receiverID(receiverID), m_content(content), m_timestamp(timestamp)
	{
		m_reply_type = ServerMsgType::SERVER_REPLY_PRIVATE_CHAT;
		m_forward_type = ServerMsgType::SERVER_FORWARD_PRIVATE_CHAT;
	};

	void execute() override;
	void send() override;
	void addHeader() override;
private:
	bool isOL = false;
	string m_receiverID;
	string m_content;
	string m_timestamp;
	ServerMsgType m_forward_type;
	Json::Value m_forward_pkt;
};