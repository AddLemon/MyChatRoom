#include "Function.h"

Function::Function()
{
	cout << "Function is ready!" << endl;
}

Function::~Function()
{
	this->allUsers.Save();
}

void Function::InitData()
{
	this->allUsers.Load();
}

void Function::LogIn()
{
	//get user`s id and password

	//get user`s info from allUsers vector

	//check if both id and password are correct
	if () {
		//correct,insert to olUsers map

		//send online user names
	}
	else {
		//incorrect, send failed msg and close connect
	}
}

void Function::LogOff()
{
	//close connect

	//remove from olUsers map
}

void Function::Register()
{
	//get new id and password

	//check if both id and password avalible (no duplication)

	if () {
		//yes, add info to allUsers vector

		//send success msg

	}
	else {
		//no, send failed msg
	}
	//close connect
}

void Function::Settings()
{
	// TODO
}

void Function::PrivateChat()
{
}

void Function::GetInstruction()
{
	//recv, now have socket, msgType, msg
	MsgType type;

	//TODO check if socket is in olUsers map. if not, send failed msg means client need to login first

	//get subType from msgType to different functions
	//
	//
}
