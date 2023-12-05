#include "Server.h"

Server* Server::m_server = new Server;	//init static member variables

Server::Server()
{	
	m_database = new SqliteController();
	m_encryptor = new SodiumEncryptor();
	//创建并初始化encryptor
	if (m_encryptor->init() == 0) {
		cout << "Server failed to init encryptor, please restart.. " << endl;
		while (1) {};
	}
	m_encryptor->generate_server_keys();		//生成非对称密钥对
	cout << "Server init success" << endl;		// test ******************
}

Server::~Server()
{
	//delete m_threadPool;
	delete m_encryptor;
	delete m_database;
	for (map<string, ClientSocket>::iterator i = m_socketMap.begin();
		i != m_socketMap.end(); ++i)
	{
		delete i->second.socket;
		delete i->second.symKey;
	}
}

bool Server::receive(Socket* sockPtr, Json::Value& deserialized_pkt, const unsigned char* symmetric_key)
{
	vector<unsigned char> encrypted_message;
	int a = sockPtr->read(encrypted_message);
	if (a == 1) {
		string decrypted_message = m_encryptor->decrypt(symmetric_key, encrypted_message);
		deserialized_pkt = deserialize(decrypted_message);
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

	//encryption and write
	map<string, ClientSocket>::iterator i = m_socketMap.find(id);
	if (i != m_socketMap.end()) {
		//user is online
		//get symmetric key and encryption
		const unsigned char* symmetric_key = i->second.symKey;		
		vector<unsigned char> encrypted_reply = m_encryptor->encrypt(symmetric_key, serialized_pkt);
		//get socket and write
		Socket* sockPtr = i->second.socket;							
		sockPtr->write(encrypted_reply);
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

Json::Value Server::dealLogin(string& tmpID, string userID, string password, Json::Value& notice_msg, queue<string>& onlineFriends)
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
			vector<pair<int, string>> db_groups;

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
				jsonObj["status"] = isOnline(pair.first);
				friends.append(jsonObj);

				if (isOnline(pair.first)) {
					onlineFriends.push(pair.first);	//get online friends
				}
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

			notice_msg["userID"] = userID;
			notice_msg["status"] = true;
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
	string userName;
	bool a = false;
	a = m_database->queryUserName(senderID, userName);
	a = m_database->insertGroup(groupID, groupName);
	a = m_database->insertGroupMember(groupID, senderID, userName);

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
		new_message["reason"] = "Failed to create the group.";
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

void Server::dealLogOff(string userID, Json::Value& notice_msg, queue<string>& onlineFriends)
{
	//query data from database
	vector<pair<string, string>> db_friends;

	m_database->queryFriendList(userID, db_friends);

	for (const auto& pair : db_friends) {
		if (isOnline(pair.first)) {
			onlineFriends.push(pair.first);	//get online friends
		}
	}

	notice_msg["userID"] = userID;
	notice_msg["status"] = false;
}

void Server::renewSocketMapKey(string oldKey, string newKey)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	auto it = m_socketMap.find(oldKey);
	if (it != m_socketMap.end()) {
		m_socketMap[newKey] = it->second;   // insert new key-value pair
		m_socketMap.erase(it);              // delete old key-value pair
	}
}

void Server::newConnect(string id, Socket* sockPtr, const unsigned char* symKey)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	ClientSocket cs = { sockPtr , symKey };
	m_socketMap.insert(pair<string, ClientSocket>(id, cs));
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
	return m_socketMap.at(id).socket;
}

const unsigned char* Server::getSymmetricKey(string id)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	return m_socketMap.at(id).symKey;
}

bool Server::isOnline(string id)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	map<string, ClientSocket>::iterator i = m_socketMap.find(id);
	if (i != m_socketMap.end()) {
		return true;
	}
	return false;
}

SodiumEncryptor* Server::getEncryptor()
{
	return m_encryptor;
}


