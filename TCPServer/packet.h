#pragma once
#include "public.h"

enum MsgType {
	name = 0,
	onlineUserNames = 1,
	singleMsg = 2,
	multiMsg = 4,
	id = 8,
	password = 16,
	serverMsg = 32
};

class Packet
{
private:
	struct Header {
		const short head = 0xFEFF;
		char checkSum;
		MsgType type;
		size_t len;
		//TODO userID, groupID;

		Header() : checkSum(0), type(name), len(0) {}		//init struct Header
	};

public:
	/// <summary>
	/// Receive message from client and save in the input container. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to current client</param>
	/// <param name="msg">A container for saving message</param>
	/// <param name="type">Type of the message</param>
	/// <returns>Return true when receive successfully, else return false.</returns>
	bool RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type);

	/// <summary>
	///	Send message to the server. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container saving message</param>
	/// <param name="type">Type of the message</param>
	/// <returns>Return true when send successfully, else return false.</returns>
	bool SendMsg(const SockPtr sockPtr, const string& msg, const MsgType type);
};
