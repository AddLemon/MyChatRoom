#pragma once
#include <iostream>
#include <string>
#include <map>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;
using asio::ip::tcp;

#define USER_ID_LENGTH 16
#define NAME_LENGTH 16
#define PASSWORD_LENGTH 16
#define MESSAGE_LENGTH 256

typedef std::shared_ptr<tcp::socket> SockPtr;

extern map<SockPtr, string> cliMap;

enum MsgType {
	serverMsg = 0,
	serverSignal = 1,
	logIn = 2,
	newAccount = 3,
	chat = 4,
	settings = 5,
	logOff = 6,
};

enum Result {
	success = 0,

	/* log in check */
	idNotExit = 1,
	passwordNotCorrect = 2,

	/* find result */
	notOnline = 3,
	receiverNotOnline = 4,

	/* register check */
	duplicateID = 5,

	/* chat check */
	receiverNotExit = 6,
};

/// <summary>
/// describe the meaning of return value
/// </summary>
/// <param name="result">result value</param>
/// <returns></returns>
string DescribeReturn(Result result);

struct User {
	string id;
	string name;
	string password;

	//operator overloading for find function to compare two User struct
	bool operator==(const User& other) const {
		return ((this->id == other.id) &&
			(this->name == other.name) &&
			(this->password == other.password));
	}
};

/// <summary>
/// Deal with client`s requirements in thread
/// </summary>
/// <param name="sockPtr">A pointer of the socket connected to current client</param>
void DealClient(SockPtr sockPtr);

#ifdef _WIN32
//UTF-8×ªÕ­×Ö·û´®
string U8toA(const string& s);
#endif