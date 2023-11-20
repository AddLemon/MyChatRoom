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
	Owner,    // Ⱥ��
	Admin,    // ����Ա
	Member    // ��ͨ��Ա
};

class SqliteController {
public:
	SqliteController();
	~SqliteController();
public:
	//��
	bool insertUser(const string& userID, const string& userName, const string& password);	//����û�
	bool insertFriend(const string& userID1, const string& userID2);	//��Ӻ��ѹ�ϵ
	bool insertGroup(int& groupID, const string& groupName);	//���Ⱥ��			//new
	bool insertGroupMember(const int& groupID, const string& userID, const string& userName);	//���Ⱥ��Ա			//new
	bool insertPrivateMessage(const string& senderID, const string& receiverID, const string& content, const string& timestamp);
	bool insertGroupMessage(const string& senderID, const string& receiverID, const int groupID, const string& content, const string& timestamp);

	//ɾ
	/// <summary>
	/// delete friendship
	/// </summary>
	/// <param name="userID1"></param>
	/// <param name="userID2"></param>
	/// <returns>�����Ƿ���ָ���ļ�¼��˳��ִ���򷵻�true��ֻ����ִ�г���ʱ�Ż᷵��false</returns>
	bool deleteFriend(const string& userID1, const string& userID2);		//ɾ������
	bool deleteGroup(const int& groupID);								//ɾ��Ⱥ��			//new
	bool deleteGroupMember(const int& groupID, const string& userID);	//�˳�Ⱥ��			//new

	//��
	bool updateUserName(const string& userID, const string& userName);	//�޸��û�����
	bool updateUserPassword(const string& userID, const string& password);	//�޸��û�����
	bool updateGroupName(const int& groupID, const string& groupName);	//�޸�Ⱥ������			//new
	bool updateGroupMemberIdentity(const int& groupID, const string& userID, const GroupMemberIdentity& identity);	//�޸�Ⱥ��Ա���	//new

	//��
	bool isUserID(const string& userID);	//��ѯ�Ƿ���ڴ�user id
	bool queryUser(const string& userID, UserInfo& result);	//��ȡ�û�������Ϣ������
	bool queryUserName(const string& userID, string& result);	//��ȡ�û�����
	bool queryUserPassword(const string& userID, string& result); //��ȡ�û�����
	bool queryFriendList(const string& userID, vector<pair<string,string>>& result);	//��ȡ�����б�
	bool queryGroupList(const string& userID, vector<pair<string,string>>& result);	//��ȡȺ���б�			//new

	bool isGroupID(const int& groupID);		////��ѯ�Ƿ���ڴ�group id			//new
	bool queryGroupName(const int& groupID, string& result);	//��ȡȺ������			//new
	bool queryGroupMembers(const int& groupID, vector<pair<string, string>>& result);	//��ȡȺ���Ա			//new

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
	void queryAllData(int table);	//���������

	//��+ɾ
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