#pragma once
#include "public.h"
#include "cliPacket.h"

class CliFunction
{
private:
	CliPacket pkg;
	SockPtr serverSock;
	User myProfile;
public:
	CliFunction();
	~CliFunction();
public:
	/// <summary>
	/// Init data
	/// </summary>
	/// <param name="sockPtr">the pointer of socket connected to the server</param>
	void Init(SockPtr sockPtr);

	/// <summary>
	/// Respond to a user's login request
	/// </summary>
	/// <returns>true for success, otherwise false</returns>
	bool LogIn();

	// Respond to a user's logoff request
	void LogOff();

	/// <summary>
	/// Respond to a user's register request
	/// </summary>
	bool Register();

	/// <summary>
	/// Respond to a connected user's request to change personal information
	/// </summary>
	bool Settings();

	/// <summary>
	/// Respond to a user's request to talk with single person
	/// </summary>
	bool PrivateChat();

	void InitialInterface();

	void MainInterface();
};

