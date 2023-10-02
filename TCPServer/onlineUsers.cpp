#include "onlineUsers.h"

OnlineUsers::OnlineUsers() {
	cout << "Online users map created!" << endl;
}

OnlineUsers::~OnlineUsers() {
	cout << "Online users map closed!" << endl;
}

bool OnlineUsers::Insert(SockPtr sock, User user)
{
	return false;
}

bool OnlineUsers::Remove(User user)
{
	return false;
}

bool OnlineUsers::RenewInfo(SockPtr sock, User user)
{
	return false;
}

