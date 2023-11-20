#include "Server.h"

Server* Server::m_server = new Server;	//init static member variables

Server::Server()
{	
	////init handler map
	//m_handlerMap[ClientMsgType::REGISTER_UP] = bind(&Server::dealRegister, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::LOG_IN] = bind(&Server::dealLogin, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::MODIFY_USER_INFO] = bind(&Server::dealModifyUserInfo, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::FIND_USER] = bind(&Server::dealFindUser, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::ADD_FRIEND] = bind(&Server::dealAddFriend, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::REMOVE_FRIEND] = bind(&Server::dealRemoveFriend, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::FIND_GROUP] = bind(&Server::dealFindGroup, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::ADD_GROUP] = bind(&Server::dealAddGroup, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::REMOVE_GROUP] = bind(&Server::dealRemoveGroup, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::CREATE_GROUP] = bind(&Server::dealCreateGroup, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::MODIFY_GROUP_INFO] = bind(&Server::dealModifyGroupInfo, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::PRIVATE_CHAT] = bind(&Server::dealPrivateChat, this, std::placeholders::_1);
	//m_handlerMap[ClientMsgType::GROUP_CHAT] = bind(&Server::dealGroupChat, this, std::placeholders::_1);

	m_database = new SqliteController();
	//m_threadPool = new ThreadPool();
	cout << "server init success" << endl;		// test ******************
}

Server::~Server()
{
	//delete m_threadPool;
	delete m_database;
	for (map<string, Socket*>::iterator i = m_socketMap.begin();
		i != m_socketMap.end(); ++i)
	{
		delete i->second;
		i->second = nullptr;
	}
}

bool Server::receive(Socket* sockPtr, Json::Value& deserialized_pkt)
{
	string pkt;
	bool a = sockPtr->read(pkt);
	if (a) {
		deserialized_pkt = deserialize(pkt);
		return true;
	}
	return false;
}

void Server::send(string id, Json::Value pkt)
{
	Json::StyledWriter sw;			//test ********************
	cout << endl << "Server send: " << endl << sw.write(pkt) << endl;	//test ********************
	//serialize
	string serialized_pkt = serialize(pkt);

	//get socket and write
	map<string, Socket*>::iterator i = m_socketMap.find(id);
	if (i != m_socketMap.end()) {
		//user is online
		Socket* sockPtr = i->second;
		sockPtr->write(serialized_pkt);
	}
}

string Server::serialize(Json::Value pkt)
{
	Json::FastWriter writer;
	string str = writer.write(pkt);
	// remove '\n' at the end of string
	if (!str.empty() && str.back() == '\n') {
		str.pop_back();
	}
	return str;
}

Json::Value Server::deserialize(string pkt)
{
	Json::Value jpkt;
	Json::Reader reader;
	if (!reader.parse(pkt.data(), jpkt)) {
		cout << "Fail: Failed to parse data." << endl;		//test **********
	}
	return jpkt;
}

Json::Value Server::dealRegister(string id, string name, string password)
{
	//check if user id is unique
	Json::Value new_message;

	if (m_database->isUserID(id)) {
		//user id already exist
		new_message["status"] = false;
		new_message["reason"] = "User ID already exist";
	}
	else {
		//user id is avaliable, insert to database
		m_database->insertUser(id, name, password);

		//modify pkt json value to send
		new_message["status"] = true;
	}

	//modify pkt json value to send
	return new_message;
}

Json::Value Server::dealLogin(string& tmpID, string userID, string password)
{	
	//check if user id is exist
	Json::Value new_message;

	if (m_database->isUserID(userID)) {
		//user id exist, select data from database
		string result;
		bool a = m_database->queryUserPassword(userID, result);
		if (a && password == result) {
			//query data from database
			UserInfo db_userInfo;
			vector<pair<string, string>> db_friends;
			vector<pair<string, string>> db_groups;

			m_database->queryUser(userID, db_userInfo);
			m_database->queryFriendList(userID, db_friends);
			m_database->queryGroupList(userID, db_groups);

			//success, modify pkt json value to send
			Json::Value data;
			Json::Value userInfo;
			Json::Value friends;
			Json::Value groups;

			userInfo["userID"] = db_userInfo.userID;
			userInfo["userName"] = db_userInfo.userName;
			userInfo["password"] = db_userInfo.password;

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
			renewSocketMapKey(tmpID, userID);
			tmpID = userID;
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

	return new_message;
}

Json::Value Server::dealModifyUserInfo(string senderID, string name, string password)
{
	//check which info to modify
	bool a = false;
	if (!name.empty()) {
		//renew userName
		a = m_database->updateUserName(senderID, name);
	}
	if (!password.empty()) {
		//renew password
		a = m_database->updateUserPassword(senderID, password);
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

	return new_message;
}

Json::Value Server::dealFindUser(string friendID)
{
	//find from database
	bool a = m_database->isUserID(friendID);

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//exist
		//query data from database
		string db_userName_friend;

		m_database->queryUserName(friendID, db_userName_friend);

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

	return new_message;
}

Json::Value Server::dealAddFriend(string senderID, string friendID, Json::Value& notice_msg)
{
	//record to database
	bool a = m_database->insertFriend(senderID, friendID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		//query data from database
		string db_userName_sender;
		string db_userName_friend;

		m_database->queryUserName(senderID, db_userName_sender);
		m_database->queryUserName(friendID, db_userName_friend);

		//success, modify pkt json value to send
		Json::Value data_back;
		data_back["userID"] = friendID;
		data_back["userName"] = db_userName_friend;
		new_message["status"] = true;
		new_message["data"] = data_back;

		notice_msg["userID"] = senderID;
		notice_msg["userName"] = db_userName_sender;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "User not found.";
	}

	return new_message;
}

Json::Value Server::dealRemoveFriend(string senderID, string friendID, Json::Value& notice_msg)
{
	//remove from database
	bool a = m_database->deleteFriend(senderID, friendID);

	//check if success
	Json::Value new_message;

	if (a) {
		//success, modify pkt json value to send
		notice_msg["userID"] = senderID;

		new_message["status"] = true;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Failed to delete.";
	}

	return new_message;
}

Json::Value Server::dealFindGroup(int groupID)
{
	//find from database
	bool a = m_database->isGroupID(groupID);

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//exist
		//query data from database
		string db_groupName;

		m_database->queryGroupName(groupID, db_groupName);

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

	return new_message;
}

Json::Value Server::dealAddGroup(string senderID, int groupID)
{
	//record to database
	string userName;
	m_database->queryUserName(senderID, userName);
	bool a = m_database->insertGroupMember(groupID, senderID, userName);

	//check if success
	Json::Value new_message;

	if (a) {
		//success
		//query data from database
		string db_groupName;

		m_database->queryGroupName(groupID, db_groupName);

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

	return new_message;
}

Json::Value Server::dealRemoveGroup(string senderID, int groupID)
{
	//record to database
	bool a = m_database->deleteGroupMember(groupID, senderID);

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

	return new_message;
}

Json::Value Server::dealGetMembers(int groupID)
{
	//find from database
	bool a = m_database->isGroupID(groupID);

	//modify pkt json value to send
	Json::Value new_message;
	if (a) {
		//exist
		//query data from database
		vector<pair<string, string>> db_members;

		m_database->queryGroupMembers(groupID, db_members);

		//success, modify pkt json value to send
		Json::Value data;
		Json::Value members;

		for (const auto& pair : db_members) {
			Json::Value jsonObj;
			jsonObj["userID"] = pair.first;
			jsonObj["userName"] = pair.second;
			members.append(jsonObj);
		}

		data["groupID"] = groupID;
		data["members"] = members;

		new_message["status"] = true;
		new_message["data"] = data;
	}
	else {
		//not exist
		new_message["status"] = false;
		new_message["reason"] = "Group not found.";
	}

	return new_message;
}

Json::Value Server::dealCreateGroup(string senderID, string groupName)
{
	//record to database
	int groupID;
	bool a = m_database->insertGroup(groupID, groupName);

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

	return new_message;
}

Json::Value Server::dealModifyGroupInfo(string senderID, int groupID, string newName)
{
	//check if user have right to modify
	Json::Value new_message;

	if (m_database->isGroupOwner(senderID, groupID)) {
		//check which info to modify
		bool a = false;
		if (!newName.empty()) {
			//renew groupName
			a = m_database->updateGroupName(groupID, newName);
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

	return new_message;
}

Json::Value Server::dealPrivateChat(string senderID, string receiverID, string content, string timestamp, bool& isOL)
{
	Json::Value new_msg;
	new_msg["content"] = content;
	new_msg["timestamp"] = timestamp;

	//check whether receiver is online
	if (isOnline(receiverID)) {
		//online
		isOL = true;
	}
	else {
		//not online
		isOL = false;
		m_database->insertPrivateMessage(senderID, receiverID, content, timestamp);
	}

	return new_msg;
}

Json::Value Server::dealGroupChat(string senderID, int groupID, string content, string timestamp, queue<string>& onlineMembers)
{
	Json::Value new_msg;
	new_msg["content"] = content;
	new_msg["timestamp"] = timestamp;

	bool a = false;

	//group exist, get all members
	vector<pair<string, string>> members;
	m_database->queryGroupMembers(groupID, members);

	//get online members and store message for offline members
	for (const auto& i : members) {
		if (isOnline(i.first)) {
			//online
			onlineMembers.push(i.first);
		}
		else {
			//offline
			m_database->insertGroupMessage(senderID, i.first, groupID, content, timestamp);
		}
	}

	return new_msg;
}

void Server::renewSocketMapKey(string oldKey, string newKey)
{
	auto it = m_socketMap.find(oldKey);
	if (it != m_socketMap.end()) {
		m_socketMap[newKey] = it->second;   // insert new key-value pair
		m_socketMap.erase(it);              // delete old key-value pair
	}
}

void Server::newConnect(string id, Socket* sockPtr)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	m_socketMap.insert(pair<string, Socket*>(id, sockPtr));
}

void Server::disConnect(string id)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	m_socketMap.erase(id);
}

Socket* Server::getSocket(string id)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	return m_socketMap.at(id);
}

bool Server::isOnline(string id)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	map<string, Socket*>::iterator i = m_socketMap.find(id);
	if (i != m_socketMap.end()) {
		return true;
	}
	return false;
}

