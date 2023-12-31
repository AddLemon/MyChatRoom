#pragma once
#include <map>
#include <queue>
#include "json/json.h"
#include "SQLiteController.h"
#include "socket.h"
#include "public.h"
#include "acceptor.h"
#include "sodiumencryptor.h"

#define TEMPORARY_ID_LENGTH 12

using RequestHandler = function<Json::Value(Json::Value)>;

struct ClientSocket {
	Socket* socket;
	const unsigned char* symKey;
};

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
	bool receive(Socket* sockPtr, Json::Value& deserialized_pkt, const unsigned char* symmetric_key);
	void send(string id, Json::Value pkt);
	string serialize(Json::Value pkt);
	Json::Value deserialize(string pkt);

	Json::Value dealRegister(string id, string name, string password);
	Json::Value dealLogin(string& tmpID, string userID, string password, Json::Value& notice_msg, queue<string>& onlineFriends);
	Json::Value dealModifyUserInfo(string senderID, string name, string password);	//需要根据users表列增加而更改
	Json::Value dealFindUser(string id);			//需要根据需求增加而更改
	Json::Value dealAddFriend(string senderID, string friendID, Json::Value& notice_msg);			//需要根据需求增加而更改
	Json::Value dealRemoveFriend(string senderID, string friendID, Json::Value& notice_msg);
	Json::Value dealFindGroup(int groupID);			//需要根据需求增加而更改
	Json::Value dealAddGroup(string senderID, int groupID);			//需要根据需求增加而更改
	Json::Value dealRemoveGroup(string senderID, int groupID);
	Json::Value dealGetMembers(int groupID);
	Json::Value dealCreateGroup(string senderID, string name);		//需要根据需求增加而更改
	Json::Value dealModifyGroupInfo(string senderID, int groupID, string name);	//需要根据groups表列增加而更改
	Json::Value dealPrivateChat(string senderID, string receiverID, string content, string timestamp, bool& isOL);
	Json::Value dealGroupChat(string senderID, int groupID, string content, string timestamp, queue<string>& onlineMembers);
	void dealLogOff(string userID, Json::Value& notice_msg, queue<string>& onlineFriends);	//获取在线好友列表

	void renewSocketMapKey(string oldKey, string newKey);

	//process socket map
	void newConnect(string id, Socket* sockPtr, const unsigned char* symKey);
	void disConnect(string id);
	Socket* getSocket(string id);
	const unsigned char* getSymmetricKey(string id);
	bool isOnline(string id);
	SodiumEncryptor* getEncryptor();

private:
	static Server* m_server;
	map<string, ClientSocket> m_socketMap;
	SqliteController* m_database;
	SodiumEncryptor* m_encryptor;
	mutex m_mutex;                // Lock m_socketMap
};



