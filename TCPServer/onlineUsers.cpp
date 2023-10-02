#include "onlineUsers.h"

OnlineUsers::OnlineUsers() {
	cout << "Online users map created!" << endl;
}

OnlineUsers::~OnlineUsers() {
	cout << "Online users map closed!" << endl;
}

bool OnlineUsers::Insert(SockPtr sock, User user)
{
	try {
		this->olUsers.insert(pair<SockPtr, User>(sock, user));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

bool OnlineUsers::Remove(User user)
{
	try {
		for (map< SockPtr, User>::iterator i = olUsers.begin(); i != olUsers.end(); ++i) {
			if ((*i).second == user) {
				olUsers.erase(i);
				break;
			}
		}
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	
	return true;
}

bool OnlineUsers::RenewInfo(SockPtr sock, User user)
{
	try {
		map< SockPtr, User>::iterator pos = olUsers.find(sock);
		if (pos == olUsers.end()) return false;			//check if sock exit in map
		(*pos).second = user;
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

User OnlineUsers::FindUser(SockPtr sock)
{
	map< SockPtr, User>::iterator pos = olUsers.find(sock);
	if (pos == olUsers.end()) return {};
	return (*pos).second;
}

SockPtr OnlineUsers::FindSocket(const char* id)
{
	for (map< SockPtr, User>::iterator i = olUsers.begin(); i != olUsers.end(); ++i) {
		if (strcmp((*i).second.id, id)) {
			return (*i).first;
		}
	}
	return NULL;
}

