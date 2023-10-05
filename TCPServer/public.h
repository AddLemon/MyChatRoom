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

struct User {
	char id[USER_ID_LENGTH];
	char name[NAME_LENGTH];
	char password[PASSWORD_LENGTH];

	//operator overloading for find function to compare two User struct
	bool operator==(const User& other) const {
		return (string(this->id) == string(other.id) &&
			string(this->name) == string(other.name) &&
			string(this->password) == string(other.password));
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