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
};
