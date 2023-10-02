#pragma once
#include "public.h"
#include "user.h"

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
	/// <param name="user">user information</param>
	/// <returns>true for success, otherwise false</returns>
	bool Remove(User user);

	/// <summary>
	/// renew user info in one of pairs in the online user map 
	/// </summary>
	/// <param name="sock">socket connect to a user</param>
	/// <param name="user">user information</param>
	/// <returns>true for success, otherwise false</returns>
	bool RenewInfo(SockPtr sock, User user);
};

