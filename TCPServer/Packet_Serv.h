#pragma once
#include <iostream>
#include "Socket_Serv.h"
#include "json/json.h"

#define PACKET_CAPACITY 300

enum MsgType {
	SERVER_REPLY,
	SERVER_NOTICE_NEW_FRIEND,
	SERVER_NOTICE_REMOVE_FRIEND,
	SERVER_FORWARD_PRIVATE_CHAT,
	SERVER_FORWARD_GROUP_CHAT,
	REGISTER_UP,
	LOG_IN,
	MODIFY_USER_INFO,
	FIND_USER,
	ADD_FRIEND,
	REMOVE_FRIEND,
	FIND_GROUP,
	ADD_GROUP,
	REMOVE_GROUP,
	CREATE_GROUP,
	MODIFY_GROUP_INFO,
	PRIVATE_CHAT,
	GROUP_CHAT,
};

class Packet_Serv
{
public:
	/// <summary>
	/// Receive message from client and save in the input container.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket connected to current client</param>
	/// <param name="msg">A container for saving message</param>
	/// <param name="type">Type of the message</param>
	/// <param name="receiverID">id of the user who will receive the message</param>
	/// <returns>Return 1 when receive successfully, false return 0.</returns>
	bool RecvMsg(const SockPtr sockPtr, string& msg, MsgType& type, string* receiverID = nullptr);

	/// <summary>
	///	Forward message from sender to the receiver.
	/// </summary>
	/// <param name="sockPtr">A pointer of the socket for receiver connected to server</param>
	/// <param name="msg">A container saving message</param>
	/// <param name="type">Type of the message</param>
	/// <param name="senderID">id of the user who send the message</param>
	/// <returns>Return 1 when send successfully, false return 0.</returns>
	bool SendMsg(const SockPtr sockPtr, const string& msg, const MsgType& type, string* senderID = nullptr);

	/// <summary>
	/// Parse the input string to Json. If success, return 1, return 0 for failed. 
	/// </summary>
	/// <param name="str">json text in string format</param>
	/// <param name="root">an object to store parsed result</param>
	/// <param name="errMsg">a buffer to store err message</param>
	/// <returns>If success, return 1, return 0 for failed.</returns>
	//int parseToJson(string& str, Json::Value& root, string* errMsg = nullptr);

	/// <summary>
	/// Write the input Json to string. If success, return 1, return 0 for failed. 
	/// </summary>
	/// <param name="root">: the json object to be write to string.</param>
	/// <param name="str">: a buffer to store string result</param>
	/// <param name="errMsg">: a buffer to store err message</param>
	/// <returns>If success, return 1, return 0 for failed.</returns>
	//int writeToString(Json::Value& root, string& str, string* errMsg = nullptr);
};

