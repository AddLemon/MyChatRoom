#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;
using asio::ip::tcp;

typedef std::shared_ptr<tcp::socket> SockPtr;

/// <summary>
/// Receive message from client and save in the input container. Return true when successful, else return false.
/// </summary>
/// <param name="sockPtr">A pointer of the socket connected to current client</param>
/// <param name="msg">A container for saving message</param>
/// <returns>Return true when receive successfully, else return false.</returns>
bool recvMsg(SockPtr sockPtr, string& msg);


/// <summary>
///	Send message to the server. Return true when successful, else return false.
/// </summary>
/// <param name="sockPtr">A pointer of the socket connected to server</param>
/// <param name="msg">A container saving message</param>
/// <returns>Return true when send successfully, else return false.</returns>
bool sendMsg(SockPtr sockPtr, const string& msg);