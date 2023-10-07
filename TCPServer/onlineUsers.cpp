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

bool OnlineUsers::Remove(SockPtr sock)
{
	try {
		for (map< SockPtr, User>::iterator i = olUsers.begin(); i != olUsers.end(); ++i) {
			if ((*i).first == sock) {
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

bool OnlineUsers::FindUser(const SockPtr sock, User& user)
{
	if (olUsers.empty()) return false;
	map< SockPtr, User>::iterator pos = olUsers.find(sock);
	if (pos == olUsers.end()) return false;
	user = (*pos).second;
	return true;
}

bool OnlineUsers::FindUser(const SockPtr sock)
{
	if (olUsers.empty()) return false;
	map< SockPtr, User>::iterator pos = olUsers.find(sock);
	if (pos == olUsers.end()) return false;
	return true;
}

bool OnlineUsers::FindSocket(const string& id, SockPtr sock)
{
	if (olUsers.empty()) return false;
	for (map< SockPtr, User>::iterator i = olUsers.begin(); i != olUsers.end(); ++i) {
		if (id == string((*i).second.id)) {
			sock = (*i).first;
			return true;
		}
	}
	return false;
}

