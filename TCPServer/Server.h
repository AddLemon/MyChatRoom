#pragma once
#include <map>
#include <queue>
#include "json/json.h"
#include "SQLiteController.h"
#include "socket.h"
#include "public.h"
#include "acceptor.h"

#define TEMPORARY_ID_LENGTH 12

using RequestHandler = function<Json::Value(Json::Value)>;

class Server
{
private:
	Server();
	~Server();
	Server(const Server& obj) = default;
	Server& operator=(const Server& obj) = default;
public:
	//ThreadPool m_threadPool; //test
	static Server* getInstance() {
		return m_server;
	}

	/// <summary>
	/// read data from socket
	/// </summary>
	/// <param name="sockPtr">pointer of the socket from which data will be read</param>
	/// <returns>return data as a Json object, or failed to read if Json object is NULL</returns>
	bool receive(Socket* sockPtr, Json::Value& deserialized_pkt);
	void send(string id, Json::Value pkt);
	string serialize(Json::Value pkt);
	Json::Value deserialize(string pkt);

	Json::Value dealRegister(string id, string name, string password);
	Json::Value dealLogin(string& tmpID, string userID, string password);
	Json::Value dealModifyUserInfo(string senderID, string name, string password);	//��Ҫ����users�������Ӷ�����
	Json::Value dealFindUser(string id);			//��Ҫ�����������Ӷ�����
	Json::Value dealAddFriend(string senderID, string friendID, Json::Value& notice_msg);			//��Ҫ�����������Ӷ�����
	Json::Value dealRemoveFriend(string senderID, string friendID, Json::Value& notice_msg);
	Json::Value dealFindGroup(int groupID);			//��Ҫ�����������Ӷ�����
	Json::Value dealAddGroup(string senderID, int groupID);			//��Ҫ�����������Ӷ�����
	Json::Value dealRemoveGroup(string senderID, int groupID);
	Json::Value dealGetMembers(int groupID);
	Json::Value dealCreateGroup(string senderID, string name);		//��Ҫ�����������Ӷ�����
	Json::Value dealModifyGroupInfo(string senderID, int groupID, string name);	//��Ҫ����groups�������Ӷ�����
	Json::Value dealPrivateChat(string senderID, string receiverID, string content, string timestamp, bool& isOL);
	Json::Value dealGroupChat(string senderID, int groupID, string content, string timestamp, queue<string>& onlineMembers);

	void renewSocketMapKey(string oldKey, string newKey);

	//process socket map
	void newConnect(string id, Socket* sockPtr);
	void disConnect(string id);
	Socket* getSocket(string id);
	bool isOnline(string id);

private:
	static Server* m_server;
	map<string, Socket*> m_socketMap;
	SqliteController* m_database;
	mutex m_mutex;                // Lock m_socketMap
};



