#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <filesystem>
#include "sqlite3.h"
using namespace std;

#define USER_ID_LENGTH 10
#define USER_PASSWORD_LENGTH 20
#define GROUP_ID_LENGTH 8
#define CONTENT_LENGTH 500

struct UserInfo {
	string userID;
	string userName;
	string password;
};

struct PrivateMsg {
	string senderID;
	string content;
	string timestamp;
};

struct GroupMsg {
	string senderID;
	int groupID = 0;
	string content;
	string timestamp;
};

enum class GroupMemberIdentity {
	Owner,    // 群主
	Admin,    // 管理员
	Member    // 普通成员
};

class SqliteController {
public:
	SqliteController();
	~SqliteController();
public:
	//增
	bool insertUser(const string& userID, const string& userName, const string& password);	//添加用户
	bool insertFriend(const string& userID1, const string& userID2);	//添加好友关系
	bool insertGroup(int& groupID, const string& groupName);	//添加群组			//new
	bool insertGroupMember(const int& groupID, const string& userID, const string& userName);	//添加群成员			//new
	bool insertPrivateMessage(const string& senderID, const string& receiverID, const string& content, const string& timestamp);
	bool insertGroupMessage(const string& senderID, const string& receiverID, const int groupID, const string& content, const string& timestamp);

	//删
	/// <summary>
	/// delete friendship
	/// </summary>
	/// <param name="userID1"></param>
	/// <param name="userID2"></param>
	/// <returns>无论是否有指定的记录，顺利执行则返回true，只有在执行出错时才会返回false</returns>
	bool deleteFriend(const string& userID1, const string& userID2);		//删除好友
	bool deleteGroup(const int& groupID);								//删除群组			//new
	bool deleteGroupMember(const int& groupID, const string& userID);	//退出群组			//new

	//改
	bool updateUserName(const string& userID, const string& userName);	//修改用户名称
	bool updateUserPassword(const string& userID, const string& password);	//修改用户密码
	bool updateGroupName(const int& groupID, const string& groupName);	//修改群组名称			//new
	bool updateGroupMemberIdentity(const int& groupID, const string& userID, const GroupMemberIdentity& identity);	//修改群成员身份	//new

	//查
	bool isUserID(const string& userID);	//查询是否存在此user id
	bool queryUser(const string& userID, UserInfo& result);	//读取用户基本信息，待定
	bool queryUserName(const string& userID, string& result);	//读取用户名称
	bool queryUserPassword(const string& userID, string& result); //读取用户密码
	bool queryFriendList(const string& userID, vector<pair<string,string>>& result);	//读取好友列表
	bool queryGroupList(const string& userID, vector<pair<string,string>>& result);	//读取群聊列表			//new

	bool isGroupID(const int& groupID);		////查询是否存在此group id			//new
	bool queryGroupName(const int& groupID, string& result);	//读取群组名称			//new
	bool queryGroupMembers(const int& groupID, vector<pair<string, string>>& result);	//读取群组成员			//new

	bool isGroupOwner(const string& userID, const int& groupID);

	/// <summary>
	/// output all data in a table
	/// </summary>
	/// <param name="table">: 
	/// 1 is for users table; 
	/// 2 is for friends table; 
	/// 3 is for groups table; 
	/// 4 is for groupMembers table; 
	/// 5 is for messages table.</param>
	void queryAllData(int table);	//输出整个表

	//查+删
	bool takePrivateMsg(const string& receiverID, vector<PrivateMsg>& messages);
	bool takeGroupMsg(const string& receiverID, vector<GroupMsg>& messages);

private:
	bool init(string& errMsg);
	bool open(string& errMsg);
	bool exec(const char* sql, string& errMsg);
	bool query(const char* sql, int(*callback)(void*, int, char**, char**), void* result, string& errMsg);	//not used
	void close();
private:
	void enableForeignKeys();
	static int callback(void*, int, char**, char**);	//not used
private:
	sqlite3* m_db;
	mutex m_mutex;
	const char* m_path = "D:\\MyWork_CPP\\MyChatRoom\\data\\MyChatRoom.db";//"MyChatRoom.db";
};