#pragma once
#include <vector>
#include "public.h"
#include "myfile.h"

class AllUsers
{
private:
	vector<User> allUsers;
public:
	AllUsers();
	~AllUsers();
public:
	/// <summary>
	/// load all users information from file
	/// </summary>
	/// <returns>true for success, otherwise false</returns>
	bool Load();

	/// <summary>
	/// save current all users info into file
	/// </summary>
	/// <returns>true for success, otherwise false</returns>
	bool Save();

	/// <summary>
	/// insert new user`s infomation
	/// </summary>
	/// <param name="user">user infomation</param>
	/// <returns>true for success, otherwise false</returns>
	bool Insert(User user);
	
	/// <summary>
	/// remove one user`s infomation
	/// </summary>
	/// <param name="user">user infomation</param>
	/// <returns>true for success, otherwise false</returns>
	bool Erase(User user);

	/// <summary>
	/// modify one user`s information
	/// </summary>
	/// <param name="user">user infomation</param>
	/// <returns>true for success, otherwise false</returns>
	bool Modify(User user);

	/// <summary>
	/// check if id and password correct
	/// </summary>
	/// <param name="id">user id</param>
	/// <param name="password">user password</param>
	/// <param name="user">user infomation</param>
	/// <returns>return enum result number</returns>
	Result CheckCorrect(const string& id, const string& password, User& user);

	/// <summary>
	/// check if the id already exit, if not exit, user` info will be recorded
	/// <param name="id">new user id</param>
	/// <param name="name">new user name</param>
	/// <param name="password">new user password</param>
	/// <returns>return enum result number</returns>
	Result CheckExit(const string& id, const string& name, const string& password);
};

