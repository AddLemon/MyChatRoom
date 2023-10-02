#pragma once
#include "public.h"

enum MsgType {
	name = 0,
	onlineNames = 1,
	sglMsg = 2,
	MltMsg = 4
};

class Packet
{
private:
	struct Header {
		short head;
		char checkSum;
		MsgType type;
		size_t len;
		//TODO userID, groupID;

		Header() : head(0xFEFF), checkSum(0), type(name), len(0) {}		//init struct Header
	};

public:
	/// <summary>
	/// Receive message from client and save in the input container. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to current client</param>
	/// <param name="msg">A container for saving message</param>
	/// <returns>Return true when receive successfully, else return false.</returns>
	bool RecvMsg(SockPtr sockPtr, string& msg);

	/// <summary>
	///	Send message to the server. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container saving message</param>
	/// <returns>Return true when send successfully, else return false.</returns>
	bool SendMsg(SockPtr sockPtr, const string& msg);
};

