#include "allUsers.h"

AllUsers::AllUsers()
{
	this->Load();
	cout << "All users information loaded!" << endl;
}

AllUsers::~AllUsers()
{
	this->Save();
	cout << "All users information saved and closed!" << endl;
}

bool AllUsers::Load()
{
	try {
		MyFile fileUser;
		User user_empty{};
		fileUser.Open("UserInfo.txt", "a+");

		for (int i = 0; !fileUser.Feof(); i++) {
			this->allUsers.push_back(user_empty);					//extend new space for more data
			fileUser.Read(&this->allUsers[i], sizeof(User));		//read data from file to buffer
		}
		fileUser.Close();
	}
	catch (std::exception& e){
		cerr << e.what() << endl;
		return false;
	}
	
	return true;
}

bool AllUsers::Save()
{
	try {
		MyFile fileUser;
		fileUser.Open("UserInfo.txt", "w");

		for (int i = 0; i< this->allUsers.size(); i++) {
			fileUser.Write(&this->allUsers[i], sizeof(User));		//write data from buffer to file
		}
		fileUser.Close();
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

bool AllUsers::Insert(User user)
{
	try {
		allUsers.push_back(user);
		this->Save();
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

bool AllUsers::Erase(User user)
{
	try {
		vector<User>::iterator pos = find(this->allUsers.begin(), this->allUsers.end(), user); //overloaded ==operator, so can compare two user struct
		this->allUsers.erase(pos);
		this->Save();
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

bool AllUsers::Modify(User user)
{	
	try {
		/* find the struct position that has the same id as input user */
		for (vector<User>::iterator pos = allUsers.begin(); pos != allUsers.end(); ++pos) {
			if (string((*pos).id) == string(user.id)) {
				(*pos) = user;	//modify the new user info to the position
				break;
			}
		}
		this->Save();
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}
