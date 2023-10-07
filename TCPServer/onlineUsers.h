#pragma once
#include "public.h"

class OnlineUsers
{
private:
	map< SockPtr, User> olUsers;
public:
	OnlineUsers();
	~OnlineUsers();
public:
	/// <summary>
	/// insert new pair of socket and user info in the online user map
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <param name="user">user information</param>
	/// <returns>true for success, otherwise false</returns>
	bool Insert(SockPtr sock, User user);

	/// <summary>
	/// remove old pair of socket and user info in the online user map
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <returns>true for not exit or removed, otherwise false</returns>
	bool Remove(SockPtr sock);

	/// <summary>
	/// renew user info in one of pairs in the online user map 
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <param name="user">user information</param>
	/// <returns>true for success, otherwise false</returns>
	bool RenewInfo(SockPtr sock, User user);

	/// <summary>
	/// find user`s infomation who use the client with sock
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <param name="user">user information</param>
	/// <returns>true for success, otherwise false</returns>
	bool FindUser(const SockPtr sock, User& user);

	/// <summary>
	/// find if the client with input socket already log in
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <returns>true for yes, otherwise false</returns>
	bool FindUser(const SockPtr sock);

	/// <summary>
	/// find user`s socket with the input id
	/// </summary>
	/// <param name="id">user`s id</param>
	/// <param name="sock">socket connect to a user</param>
	/// <returns>true for success, otherwise false</returns>
	bool FindSocket(const string& id, SockPtr sock);
};

