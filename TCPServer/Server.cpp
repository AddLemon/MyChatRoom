#include "Server.h"

Server* Server::m_server = new Server;	//init static member variables

Server::Server()
{	
	//init handler map
	m_handlerMap[REGISTER_UP] = bind(&Server::dealRegister, this, std::placeholders::_1);
	m_handlerMap[LOG_IN] = bind(&Server::dealLogin, this, std::placeholders::_1);
	m_handlerMap[MODIFY_USER_INFO] = bind(&Server::dealModifyUserInfo, this, std::placeholders::_1);
	m_handlerMap[FIND_USER] = bind(&Server::dealFindUser, this, std::placeholders::_1);
	m_handlerMap[ADD_FRIEND] = bind(&Server::dealAddFriend, this, std::placeholders::_1);
	m_handlerMap[REMOVE_FRIEND] = bind(&Server::dealRemoveFriend, this, std::placeholders::_1);
	m_handlerMap[FIND_GROUP] = bind(&Server::dealFindGroup, this, std::placeholders::_1);
	m_handlerMap[ADD_GROUP] = bind(&Server::dealAddGroup, this, std::placeholders::_1);
	m_handlerMap[REMOVE_GROUP] = bind(&Server::dealRemoveGroup, this, std::placeholders::_1);
	m_handlerMap[CREATE_GROUP] = bind(&Server::dealCreateGroup, this, std::placeholders::_1);
	m_handlerMap[MODIFY_GROUP_INFO] = bind(&Server::dealModifyGroupInfo, this, std::placeholders::_1);
	m_handlerMap[PRIVATE_CHAT] = bind(&Server::dealPrivateChat, this, std::placeholders::_1);
	m_handlerMap[GROUP_CHAT] = bind(&Server::dealGroupChat, this, std::placeholders::_1);

	cout << "server init success" << endl;		// test ******************
}

Json::Value Server::dispatchRequest(const MsgType type, Json::Value pkt)
{
	if (type != SERVER_REPLY) {
		return m_handlerMap[type](pkt);
	}
	// todo unknown type
}

Json::Value Server::dealRegister(Json::Value pkt)
{
	//extract temporary id
	string tmpID = pkt["header"]["senderID"].asString();

	//get user id , name and message from json value
	string userID = pkt["message"]["userID"].asString();
	string userName = pkt["message"]["userName"].asString();
	string password = pkt["message"]["password"].asString();

	//check if user id is unique
	Json::Value new_message;

	if (m_database.isUserID(userID)) {
		//user id already exist
		
		new_message["status"] = false;
		new_message["reason"] = "User ID already exist";
	}
	else {
		//user id is avaliable, insert to database
		m_database.insertUser(userID, userName, password);

		//modify pkt json value to send
		new_message["status"] = true;
	}

	//modify pkt json value to send
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealLogin(Json::Value pkt)
{
	//extract temporary id
	string tmpID = pkt["header"]["senderID"].asString();

	//get user id and message from json value
	string userID = pkt["message"]["userID"].asString();
	string password = pkt["message"]["password"].asString();
	
	//check if user id is exist
	Json::Value new_message;

	if (m_database.isUserID(userID)) {
		//user id exist, select data from database
		string result;
		bool a = m_database.queryUserPassword(userID, result);
		if (a && password == result) {
			//query data from database
			string db_userName;
			vector<pair<string, string>> db_friends;
			vector<pair<string, string>> db_groups;

			m_database.queryUserName(userID, db_userName);
			m_database.queryFriendList(userID, db_friends);
			m_database.queryGroupList(userID, db_groups);

			//success, modify pkt json value to send
			Json::Value data;
			Json::Value userInfo;
			Json::Value friends;
			Json::Value groups;

			userInfo["userName"] = db_userName;

			for (const auto& pair : db_friends) {
				Json::Value jsonObj;
				jsonObj["userID"] = pair.first;
				jsonObj["userName"] = pair.second;
				friends.append(jsonObj);
			}

			for (const auto& pair : db_groups) {
				Json::Value jsonObj;
				jsonObj["groupID"] = pair.first;
				jsonObj["groupName"] = pair.second;
				groups.append(jsonObj);
			}

			data["userInfo"] = userInfo;
			data["friends"] = friends;
			data["groups"] = groups;

			new_message["status"] = true;
			new_message["data"] = data;

			//renew senderID and socketMap
			pkt["header"]["senderID"] = userID;
			renewSocketMapKey(tmpID, userID);
		}
		else {
			//password is not correct
			new_message["status"] = false;
			new_message["reason"] = "Password is incorrect.";
		}
	}
	else {
		//user id not exist
		new_message["status"] = false;
		new_message["reason"] = "User ID is incorrect.";
	}

	//modify pkt json value to send
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealModifyUserInfo(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get message from json value
	Json::Value message = pkt["message"];
	
	//check which info to modify
	bool a = false;
	if (message.isMember("userName")) {
		//renew userName
		string userName = message["userName"].asString();
		a = m_database.updateUserName(senderID, userName);
	}
	if (message.isMember("password")) {
		//renew password
		string password = message["password"].asString();
		a = m_database.updateUserPassword(senderID, password);
	}
	/* add on */

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//modify success
		new_message["status"] = true;
	}
	else {
		//modify failed
		new_message["status"] = false;
		new_message["reason"] = "Update failed.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealFindUser(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get friendID that need to find from json value
	string friendID = pkt["message"]["userID"].asString();

	//find from database
	bool a = m_database.isUserID(friendID);

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//exist
		//query data from database
		string db_userName_friend;

		m_database.queryUserName(friendID, db_userName_friend);

		//success, modify pkt json value to send
		Json::Value data;

		data["userID"] = friendID;
		data["userName"] = db_userName_friend;

		new_message["status"] = true;
		new_message["data"] = data;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "User not found.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealAddFriend(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get userID that need to find from json value
	string friendID = pkt["message"]["friendID"].asString();

	//record to database
	bool a = m_database.insertFriend(senderID, friendID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		//query data from database
		string db_userName_sender;
		string db_userName_friend;

		m_database.queryUserName(senderID, db_userName_sender);
		m_database.queryUserName(friendID, db_userName_friend);

		//success, modify pkt json value to send
		Json::Value data_back;
		Json::Value data_toFriend;

		data_back["userID"] = friendID;
		data_back["userName"] = db_userName_friend;
		data_toFriend["userID"] = senderID;
		data_toFriend["userName"] = db_userName_sender;

		new_message["status"] = true;
		new_message["data"] = data_back;

		/* forward pkt to friend */
		Json::Value pkt_toFriend = pkt;
		pkt_toFriend["header"]["type"] = SERVER_NOTICE_NEW_FRIEND;
		pkt_toFriend["header"]["receiverID"] = friendID;
		pkt_toFriend["message"] = data_toFriend;
		//notice friend
		// TODO ***********************************
		// check if friend is noline
		//send();
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "User not found.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealRemoveFriend(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get userID that need to find from json value
	string friendID = pkt["message"]["userID"].asString();

	//remove from database
	bool a = m_database.deleteFriend(senderID, friendID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success, modify pkt json value to send
		Json::Value data_toFriend;
		data_toFriend["userID"] = senderID;

		new_message["status"] = true;

		/* forward pkt to friend */
		Json::Value pkt_toFriend = pkt;
		pkt_toFriend["header"]["type"] = SERVER_NOTICE_REMOVE_FRIEND;
		pkt_toFriend["header"]["receiverID"] = friendID;
		pkt_toFriend["message"] = data_toFriend;
		//notice friend
		// TODO ***********************************
		// check if friend is noline
		//send();
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Failed to delete.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealFindGroup(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get groupID that need to find from json value
	int groupID = pkt["message"]["groupID"].asInt();

	//find from database
	bool a = m_database.isGroupID(groupID);

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//exist
		//query data from database
		string db_groupName;

		m_database.queryGroupName(groupID, db_groupName);

		//success, modify pkt json value to send
		Json::Value data;

		data["groupID"] = groupID;
		data["groupName"] = db_groupName;

		new_message["status"] = true;
		new_message["data"] = data;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Group not found.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealAddGroup(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get groupID that need to find from json value
	int groupID = pkt["message"]["groupID"].asInt();

	//record to database
	bool a = m_database.insertGroupMember(groupID, senderID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		//query data from database
		string db_groupName;

		m_database.queryGroupName(groupID, db_groupName);

		//success, modify pkt json value to send
		Json::Value data;

		data["groupID"] = groupID;
		data["groupName"] = db_groupName;

		new_message["status"] = true;
		new_message["data"] = data;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Failed to join the group.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealRemoveGroup(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get groupID that need to find from json value
	int groupID = pkt["message"]["groupID"].asInt();

	//record to database
	bool a = m_database.deleteGroupMember(groupID, senderID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		new_message["status"] = true;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Failed to leave the group.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealCreateGroup(Json::Value pkt)
{
	//get senderID from json value
	string senderID = pkt["header"]["senderID"].asString();

	//get groupName that need to find from json value
	string groupName = pkt["message"]["groupName"].asString();

	//record to database
	int groupID;
	bool a = m_database.insertGroup(groupID, senderID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		Json::Value data;
		data["groupID"] = groupID;
		data["groupName"] = groupName;

		new_message["status"] = true;
		new_message["data"] = data;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Failed to leave the group.";
	}
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealModifyGroupInfo(Json::Value pkt)
{
	//get userID from json value
	string userID = pkt["header"]["senderID"].asString();

	//get message from json value
	int groupID = pkt["message"]["groupID"].asInt();
	Json::Value newInfo = pkt["message"]["newInfo"];

	//check if user have right to modify
	Json::Value new_message;

	if (m_database.isGroupOwner(userID, groupID)) {
		//check which info to modify
		bool a = false;
		if (newInfo.isMember("groupName")) {
			//renew groupName
			string groupName = newInfo["groupName"].asString();
			a = m_database.updateGroupName(groupID, groupName);
		}
		/* add on */

		//modify pkt json value to send
		if (a) {
			//modify success
			new_message["status"] = true;
		}
		else {
			//modify failed
			new_message["status"] = false;
			new_message["reason"] = "Update failed.";
		}
	}
	else {
		//no right to modify
		new_message["status"] = false;
		new_message["reason"] = "No permission to modify.";
	}
	
	pkt["message"] = new_message;

	return pkt;
}

Json::Value Server::dealPrivateChat(Json::Value pkt)
{
	return Json::Value();
}

Json::Value Server::dealGroupChat(Json::Value pkt)
{
	return Json::Value();
}

void Server::feedback(Json::Value new_pkt)
{
	//get receiver`s socket
	SockPtr recvSock = nullptr;
	string senderID = new_pkt["header"]["senderID"].asString();
	if (m_socketMap.find(senderID) != m_socketMap.end()) {
		//client is online
		recvSock = m_socketMap[senderID];
	}
	else {
		//client is offline
		return;
	}

	//modify msg type
	new_pkt["header"]["type"] = SERVER_REPLY;

	//send feedback
	//sendMsg(recvSock, new_pkt);


	
}

void Server::renewSocketMapKey(string oldKey, string newKey)
{
	auto it = m_socketMap.find(oldKey);
	if (it != m_socketMap.end()) {
		m_socketMap[newKey] = it->second;   // insert new key-value pair
		m_socketMap.erase(it);              // delete old key-value pair
	}
}