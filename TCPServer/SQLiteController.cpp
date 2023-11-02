#include "SQLiteController.h"

SqliteController::SqliteController() {
	m_db = nullptr;
	string errMsg;

	if (!open(errMsg)) {
		cout << "Failed: " << errMsg << endl;
	}
	cout << "Success to open database." << endl;		//test ***********
}

SqliteController::~SqliteController() {
	close();
}

bool SqliteController::insertUser(const string& userID, const string& userName, const string& password)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_insert_user = "INSERT INTO users (userID, username, password) VALUES (?, ?, ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_insert_user, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for insert user." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userName.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 3, password.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: insert user." << endl;		//test ***********
		return false;
	}
	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::insertFriend(const string& userID1, const string& userID2)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_insert_friend = "INSERT INTO friends (userID1, userID2) VALUES (?, ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_insert_friend, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for insert friends." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID1.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userID2.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: insert friends ( " << sqlite3_errmsg(m_db) << " )" << endl;		//test ***********
		return false;
	}
	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::insertGroup(int& groupID, const string& groupName)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_insert_group = "INSERT INTO groups (groupName) VALUES (?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_insert_group, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for insert group." << sqlite3_errmsg(m_db) << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: insert group ( " << sqlite3_errmsg(m_db) << " )" << endl;		//test ***********
		return false;
	}

	//get automatically generated groupID
	groupID = static_cast<int>(sqlite3_last_insert_rowid(m_db));

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::insertGroupMember(const int& groupID, const string& userID)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_insert_groupMember = "INSERT INTO groupMembers (groupID, userID, identity) VALUES (?, ?, ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_insert_groupMember, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for insert group member." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);
	sqlite3_bind_text(stmt, 2, userID.c_str(), -1, NULL);
	sqlite3_bind_int(stmt, 3, static_cast<int>(GroupMemberIdentity::Member));

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: insert group member( " << sqlite3_errmsg(m_db) << " )" << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::deleteFriend(const string& userID1, const string& userID2)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_delete_friend = "DELETE FROM friends WHERE (userID1 = ? AND userID2 = ?) OR (userID1 = ? AND userID2 = ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_delete_friend, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for delete friends." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID1.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userID2.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 3, userID2.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 4, userID1.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: delete friends." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::deleteGroup(const int& groupID)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_delete_group = "DELETE FROM groups WHERE groupID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_delete_group, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for delete group." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: delete group." << endl;		//test ***********
		return false;
	}
	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::deleteGroupMember(const int& groupID, const string& userID)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_delete_groupMember = "DELETE FROM groupMembers WHERE (groupID = ? AND userID = ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_delete_groupMember, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for delete a group member." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);
	sqlite3_bind_text(stmt, 2, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: delete a group member." << endl;		//test ***********
		return false;
	}
	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::updateUserName(const string& userID, const string& userName)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_update_userName = "UPDATE users SET userName = ? WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_update_userName, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for update user name." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userName.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: update user name." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::updateUserPassword(const string& userID, const string& password)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_update_password = "UPDATE users SET password = ? WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_update_password, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for update user password." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, password.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: update user password." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::updateGroupName(const int& groupID, const string& groupName)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_update_groupName = "UPDATE groups SET groupName = ? WHERE groupID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_update_groupName, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for update user name." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, NULL);
	sqlite3_bind_int(stmt, 2, groupID);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: update user name." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::updateGroupMemberIdentity(const int& groupID, const string& userID, const GroupMemberIdentity& identity)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_update_groupMember_identity = "UPDATE groupMembers SET identity = ? WHERE (groupID = ? AND userID = ?)";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_update_groupMember_identity, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for update a group member`s identity." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, static_cast<int>(identity));
	sqlite3_bind_int(stmt, 2, groupID);
	sqlite3_bind_text(stmt, 3, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		cout << "Failed execution: update a group member`s identity." << endl;		//test ***********
		return false;
	}
	sqlite3_finalize(stmt);

	return true;
}

bool SqliteController::isUserID(const string& userID)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_exist_userID = "SELECT 1 FROM users WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_exist_userID, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for finding if userID exist." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//userID exist
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//userID not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: find if userID exist." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryUser(const string& userID, userInfo& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_user = "SELECT * FROM users WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_user, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting user." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//user exist
		result.userID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		result.userName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		result.password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//user not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select user." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryUserName(const string& userID, string& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_userName = "SELECT userName FROM users WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_userName, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting userName." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//user exist
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//user not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select userName." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryUserPassword(const string& userID, string& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_password = "SELECT password FROM users WHERE userID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_password, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting password." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//user exist
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//user not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select password." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryFriendList(const string& userID, vector<pair<string, string>>& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_friends =
		"SELECT userID1 FROM friends WHERE userID2 = ? "
		"UNION "
		"SELECT userID2 FROM friends WHERE userID1 = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_friends, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting friends." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		//user exist
		string userID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		string userName;
		queryUserName(userID, userName);
		result.push_back(pair(userID, userName));
	}

	if (rc != SQLITE_DONE) {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select friends." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

bool SqliteController::queryGroupList(const string& userID, vector<pair<string, string>>& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_groups = "SELECT groupID FROM groupMembers WHERE userID = ? ";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_groups, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting entered groups." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_text(stmt, 1, userID.c_str(), -1, NULL);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		//user exist
		string groupID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		string groupName;
		queryUserName(groupID, groupName);
		result.push_back(pair(groupID, groupName));
	}

	if (rc != SQLITE_DONE) {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select entered groups." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

bool SqliteController::isGroupID(const int& groupID)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_exist_groupID = "SELECT 1 FROM groups WHERE groupID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_exist_groupID, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for finding if groupID exist." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//userID exist
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//userID not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: find if groupID exist." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryGroupName(const int& groupID, string& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_groupName = "SELECT groupName FROM groups WHERE groupID = ?";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_groupName, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting groupName." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		//user exist
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sqlite3_finalize(stmt);
		return true;
	}
	else if (rc == SQLITE_DONE) {
		//user not exist
		sqlite3_finalize(stmt);
		return false;
	}
	else {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select groupName." << endl;		//test ***********
		return false;
	}
}

bool SqliteController::queryGroupMembers(const int& groupID, vector<pair<string, GroupMemberIdentity>>& result)
{
	int rc = SQLITE_ERROR;
	sqlite3_stmt* stmt;
	const char* sql_select_groupMembers = "SELECT userID, identity FROM groupMembers WHERE groupID = ? ";

	//prepare sql
	rc = sqlite3_prepare_v2(m_db, sql_select_groupMembers, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cout << "Failed to prepare statement for selecting members of a group." << endl;		//test ***********
		return false;
	}

	//bind variables
	sqlite3_bind_int(stmt, 1, groupID);

	//execute
	unique_lock<mutex> lock(m_mutex);	//lock
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		//member exist
		string userID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		GroupMemberIdentity identity = static_cast<GroupMemberIdentity>(sqlite3_column_int(stmt, 1));
		result.push_back({ userID, identity });
	}

	if (rc != SQLITE_DONE) {
		//err
		sqlite3_finalize(stmt);
		cout << "Failed execution: select members of a group." << endl;		//test ***********
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

bool SqliteController::isGroupOwner(const string& userID, const int& groupID)
{
	return false;
}

void SqliteController::queryAllData(int table)
{
	int rc = SQLITE_ERROR;
	char** result = NULL;
	int rowCount, columnCount;
	char* errMsg;

	//拼接字符串
	string sql = "SELECT * FROM ";
	switch (table) {
	case 1:
		sql += "users"; break;
	case 2:
		sql += "friends"; break;
	case 3:
		sql += "groups"; break;
	case 4:
		sql += "groupMembers"; break;
	case 5:
		sql += "messages"; break;
	default:
		cout << "Failed execution: table number is wrong to query all data." << endl;
		return;
	}

	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_get_table(m_db,		//连接句柄
		sql.c_str(),					//SQL语句
		&result,						//out: 结果集（包含了字段和值）
		&rowCount,						//数据行数（记录数）
		&columnCount,					//列数
		&errMsg							//错误信息
	);
	if (rc != SQLITE_OK) {
		cout << "Failed to query all data: " << sqlite3_errmsg(m_db) << endl;
	}
	else {
		cout << "row: " << rowCount << " " << "column: " << columnCount << endl;
		for (size_t r = 0; r < rowCount + 1; r++) {			//行数只是记录的数量，还需要加上表头的一行
			for (size_t c = 0; c < columnCount; c++) {
				cout << result[r * columnCount + c] << " ";
			}
			cout << endl;
		}
	}
}

bool SqliteController::init(string& errMsg) {
	int rc = SQLITE_ERROR;

	//open database
	rc = sqlite3_open(DP_PATH.c_str(), &m_db);
	if (rc != SQLITE_OK) {
		errMsg = "Create database failed.";	//write errMsg
		return false;
	}

	//create initial users tables
	if (!exec("\
			CREATE TABLE users (\
			userID		CHAR(10) NOT NULL,\
			userName	CHAR(20) NOT NULL,\
			password	CHAR(20) NOT NULL,\
			PRIMARY KEY (userID)\
			)\
		", errMsg)) return false;

	//create initial friends tables
	if (!exec("\
			CREATE TABLE friends (\
			userID1		CHAR(10) NOT NULL,\
			userID2		CHAR(10) NOT NULL,\
			UNIQUE(userID1, userID2),\
			FOREIGN KEY (userID1) REFERENCES users (userID),\
			FOREIGN KEY (userID2) REFERENCES users (userID)\
			)\
		", errMsg)) return false;

	//create initial groups tables and 
	if (!exec("\
			CREATE TABLE groups (\
			groupID		INTEGER PRIMARY KEY AUTOINCREMENT,\
			groupName	CHAR(20)	NOT NULL\
			)\
		", errMsg)) return false;
	//set initial value of groupID
	if (!exec("INSERT INTO groups (groupID, groupName) VALUES (10000000, 'test')", errMsg)) return false;
	if (!exec("DELETE FROM groups WHERE groupName = 'test'", errMsg)) return false;

	//create initial groupMembers tables
	if (!exec("\
			CREATE TABLE groupMembers (\
			groupID		INT		 NOT NULL,\
			userID		CHAR(10) NOT NULL,\
			identity	INT		 DEFAULT 2,\
			UNIQUE(groupID, userID),\
			FOREIGN KEY (groupID) REFERENCES groups (groupID)  ON DELETE CASCADE,\
			FOREIGN KEY (userID) REFERENCES users (userID)\
			)\
		", errMsg)) return false;

	//create initial messages tables
	if (!exec("\
			CREATE TABLE messages (\
			senderID	CHAR(10)		NOT NULL,\
			receiverID	CHAR(10)		NOT NULL,\
			content		VARCHAR(500)	NOT NULL,\
			timestamp	DATETIME		DEFAULT CURRENT_TIMESTAMP,\
			FOREIGN KEY (senderID) REFERENCES users (userID),\
			FOREIGN KEY (receiverID) REFERENCES users (userID)\
			)\
		", errMsg)) return false;

	cout << "success to init database" << endl;
	return true;
}

bool SqliteController::open(string& errMsg) {
	int rc = SQLITE_ERROR;

	if (!filesystem::exists(DP_PATH)) {
		//create database and init table
		if (!init(errMsg)) {
			return false;
		}
	}
	else {
		rc = sqlite3_open(DP_PATH.c_str(), &m_db);
		if (rc != SQLITE_OK) {
			errMsg = "Open database failed.";	//write errMsg
			return false;
		}
	}
	enableForeignKeys();

	return true;
}

bool SqliteController::exec(const char* sql, string& errMsg) {
	int rc = SQLITE_ERROR;

	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_exec(m_db, sql, NULL, NULL, NULL);
	lock.unlock();	//unlock

	if (rc != SQLITE_OK) {
		errMsg = sqlite3_errmsg(m_db);	//return err message
		return false;
	}
	return true;
}

bool SqliteController::query(const char* sql, int(*callback)(void*, int, char**, char**), void* result, string& errMsg) {
	int rc = SQLITE_ERROR;

	unique_lock<mutex> lock(m_mutex);	//lock
	rc = sqlite3_exec(m_db, "SELECT * FROM students", callback, result, NULL);		//每查询到一条数据就会调用一次callback函数
	lock.unlock();	//unlock

	if (rc != SQLITE_OK) {
		errMsg = sqlite3_errmsg(m_db);
		return false;
	}
	return true;
}

void SqliteController::close() {
	//close database
	sqlite3_close(m_db);
}

void SqliteController::enableForeignKeys()
{
	if (!m_db) return;

	char* errMsg = nullptr;
	int rc = sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsg);

	if (rc != SQLITE_OK) {
		cout << "SQL error: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}

int SqliteController::callback(void*, int, char**, char**)
{
	return 0;
}
