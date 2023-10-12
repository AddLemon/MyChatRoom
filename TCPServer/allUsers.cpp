#include "allUsers.h"

AllUsers::AllUsers()
{
	Load();
	cout << "All users information loaded!" << endl;
}

AllUsers::~AllUsers()
{
	Save();
	cout << "All users information saved and closed!" << endl;
}

bool AllUsers::Load()
{
	//open file to get data
	ifstream in;
	in.open("..\\..\\data\\allUsersInfo.json", ios::in | ios::binary);

	if (!in.is_open())
	{
		cout << "Error opening file" << endl;	//return signal
		return false;
	}

	//parse to json
	Json::Value allUsers_json;
	Json::Reader reader;

	if (reader.parse(in, allUsers_json))
	{
		//read every user info from json to vector
		for (unsigned int i = 0; i < allUsers_json.size(); i++) {
			User user{};
			user.id = allUsers_json[i]["id"].asString();
			user.name = allUsers_json[i]["name"].asString();
			user.password = allUsers_json[i]["password"].asString();
			allUsers.push_back(user);
		}
		//TODO return signal
	}
	else {
		cout << "allUsers parse err" << endl;			//return signal
	}

	in.close();
	return true;
}

bool AllUsers::Save()
{
	Json::Value allUsers_json;
	Json::StyledWriter swriter;

	//store vector to json
	for (vector<User>::iterator i = allUsers.begin(); i != allUsers.end(); ++i) {
		Json::Value user_json;
		user_json["id"] = (*i).id;
		user_json["name"] = (*i).name;
		user_json["password"] = (*i).password;
		allUsers_json.append(user_json);
	}

	//open file
	ofstream out;
	out.open("..\\..\\data\\allUsersInfo.json", ios::binary|ios::out);

	//write json to file
	out << swriter.write(allUsers_json);
	out.close();

	return true;
}

bool AllUsers::Insert(User user)
{
	try {
		allUsers.push_back(user);
		Save();
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
		Save();
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

Result AllUsers::CheckCorrect(const string& id, const string& password, User& user)
{
	Result result = idNotExit;
	try {
		//traverse the container to check id and password
		for (vector<User>::iterator i = this->allUsers.begin(); i != allUsers.end(); ++i) {
			if (string((*i).id) == id) {
				if (string((*i).password) == password) {
					user = *i;
					result = success;
				}
				else result = passwordNotCorrect;
			}
			else continue;
		}
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
	}
	return result;
}

Result AllUsers::CheckExit(const string& id, const string& name, const string& password)
{
	// traverse to check if input id already exit
	for (vector<User>::iterator i = allUsers.begin(); i != allUsers.end(); ++i) {
		if ((*i).id == id) return duplicateID;
	}
	// insert into allUsers vector
	User user = { id, name, password };
	Insert(user);
	return success;
}
