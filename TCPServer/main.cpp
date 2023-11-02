#include "Packet_Serv.h"
#include "Server.h"
#include "SQLiteController.h"
#include "ThreadPool.h"

int main()
{
	/************************************************************************/
	Server* server = Server::getInstance();

	string tmpID = to_string(124578);
	MsgType type = REGISTER_UP;
	Json::Value pkt;
	pkt["header"]["requireID"] = 123456789;
	pkt["header"]["type"] = REGISTER_UP;
	pkt["header"]["senderID"] = "124578";
	pkt["header"]["receiverID"];
	pkt["header"]["groupID"];
	pkt["message"]["userID"] = "123456";
	pkt["message"]["userName"] = "Alex";
	pkt["message"]["password"] = "alex123456";


	Json::Value msg = pkt["message"];	//接收到的信息，传入线程

	Json::StyledWriter styledWriter;
	std::cout << styledWriter.write(pkt);

	auto func = [&]() {
		Json::Value result = server->dispatchRequest(type, pkt);
		std::cout << styledWriter.write(result);
	};
	Task task(func);

	server->m_threadPool.addTask(&task);

	this_thread::sleep_for(chrono::seconds(100));
}