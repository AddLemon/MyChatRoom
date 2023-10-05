#pragma once
#include "onlineUsers.h"
#include "allUsers.h"
#include "packet.h"

class Function
{
private:
	AllUsers allUsers;
	OnlineUsers olUsers;
public:
	Function();
	~Function();
public:
	// create user data containers and load data from files
	void InitData();

	// Respond to a user's login request
	void LogIn();

	// Respond to a user's logoff request
	void LogOff();

	// Respond to a user's register request
	void Register();

	// Respond to a connected user's request to change personal information
	void Settings();

	// Respond to a user's request to talk with single person
	void PrivateChat();

	void GetInstruction();
};

