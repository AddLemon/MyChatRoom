#pragma once
#include "onlineUsers.h"
#include "allUsers.h"
#include "packet.h"

class Function
{
private:
	AllUsers allUsers;
	OnlineUsers olUsers;
	Packet pkg;
public:
	Function();
	~Function();
public:
	// create user data containers and load data from files
	void InitData();

	/// <summary>
	/// Respond to a user's login request
	/// </summary>
	/// <param name="sockPtr">socket connect to a user</param>
	/// <param name="recvMsg">the content about the request</param>
	void LogIn(SockPtr sockPtr, const string& recvMsg);

	// Respond to a user's logoff request
	void LogOff(SockPtr sockPtr);

	/// <summary>
	/// Respond to a user's register request
	/// </summary>
	/// <param name="sockPtr">socket connect to a user</param>
	/// <param name="recvMsg">the content about the request</param>
	void Register(SockPtr sockPtr, const string& recvMsg);

	/// <summary>
	/// Respond to a connected user's request to change personal information
	/// </summary>
	/// <param name="sockPtr">socket connect to a user</param>
	/// <param name="recvMsg">the content about the request</param>
	void Settings(SockPtr sockPtr, const string& recvMsg);

	/// <summary>
	/// Respond to a user's request to talk with single person
	/// </summary>
	/// <param name="sockPtr">socket connect to a sender client</param>
	/// <param name="recvMsg">the content of the message to send</param>
	/// <param name="receiverID">the id of a user who will receive the message</param>
	void PrivateChat(SockPtr sockPtr, const string& recvMsg, const string& receiverID);

	void GetInstruction(SockPtr sockPtr);

	/// <summary>
	/// Deal with client`s requirements in thread
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to current client</param>
	void DealClient(SockPtr sockPtr);
};

