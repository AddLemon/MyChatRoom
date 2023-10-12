#pragma once
#include "public.h"
#include "json/json.h"

class CliPacket
{
public:
	/// <summary>
	/// Receive message from server and save in the input container. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container for saving message</param>
	/// <param name="type">Type of the message</param>
	/// <param name="receiverID">id of the user who send the message</param>
	/// <returns>Return true when receive successfully, else return false.</returns>
	bool RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type, string& receiverID);

	/// <summary>
	/// Receive message from server and save in the input container. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container for saving message</param>
	/// <param name="type">Type of the message</param>
	/// <returns>Return true when receive successfully, else return false.</returns>
	bool RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type);

	/// <summary>
	///	Send message to the receiver through server. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container saving message</param>
	/// <param name="type">Type of the message</param>
	/// <param name="senderID">id of the user who will receive the message</param>
	/// <returns>Return true when send successfully, else return false.</returns>
	bool SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type, const string& senderID);

	/// <summary>
	/// Send message to the receiver through server. Return true when successful, else return false.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to server</param>
	/// <param name="msg">A container saving message</param>
	/// <param name="type">Type of the message</param>
	/// <returns>Return true when send successfully, else return false.</returns>
	bool SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type);

	void JtoStr();
};

