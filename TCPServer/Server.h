#pragma once
#include <map>
#include "Packet_Serv.h"
#include "SQLiteController.h"
#include "ThreadPool.h"

#define TEMPORARY_ID_LENGTH 12

using RequestHandler = function<Json::Value(Json::Value)>;

class Server
{
public:
	ThreadPool m_threadPool; //test
	static Server* getInstance() {
		return m_server;
	}

	// execute mapped function
	Json::Value dispatchRequest(const MsgType type, Json::Value pkt);
	void feedback(Json::Value new_pkt);
private:
	Json::Value dealRegister(Json::Value pkt);
	Json::Value dealLogin(Json::Value pkt);
	Json::Value dealModifyUserInfo(Json::Value pkt);	//需要根据users表列增加而更改
	Json::Value dealFindUser(Json::Value pkt);			//需要根据需求增加而更改
	Json::Value dealAddFriend(Json::Value pkt);			//需要根据需求增加而更改
	Json::Value dealRemoveFriend(Json::Value pkt);
	Json::Value dealFindGroup(Json::Value pkt);			//需要根据需求增加而更改
	Json::Value dealAddGroup(Json::Value pkt);			//需要根据需求增加而更改
	Json::Value dealRemoveGroup(Json::Value pkt);
	Json::Value dealCreateGroup(Json::Value pkt);		//需要根据需求增加而更改
	Json::Value dealModifyGroupInfo(Json::Value pkt);	//需要根据groups表列增加而更改
	Json::Value dealPrivateChat(Json::Value pkt);
	Json::Value dealGroupChat(Json::Value pkt);

	void renewSocketMapKey(string oldKey, string newKey);
private:
	Server();
	Server(const Server& obj) = default;
	Server& operator=(const Server& obj) = default;
private:
	static Server* m_server;
	map<string, SockPtr> m_socketMap;
	unordered_map<MsgType, RequestHandler> m_handlerMap;		// Mapping of msg types and execution functions

	SqliteController m_database;
	//ThreadPool m_threadPool;
};



