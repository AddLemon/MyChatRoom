#include "acceptor.h"
#include "Server.h"
#include "SQLiteController.h"
#include "ThreadPool.h"
#include "dispatcher.h"
#include <sdkddkver.h>

int main()
{
	/************************************************************************/
	Server* server = Server::getInstance();
	ThreadPool* tp = new ThreadPool();
	Dispatcher* disp = new Dispatcher(tp);

	Acceptor acceptor = Acceptor();

	//create temperary id
	int tmp = 100000;
	while (true) {
		tmp++;
		//accept connection
		Socket* s = acceptor.createSocket();
		acceptor.accept(s);
		cout << "connected!" << endl;
		
		//sub thread
		thread myThread([&, tmp, s]() {
			string tmpID = to_string(tmp);
			server->newConnect(tmpID, s);	//record sender id with socket pointer

			////test *************************
			//ClientMsgType type = ClientMsgType::REGISTER_UP;
			//Json::Value jsonpkt;
			//jsonpkt["header"]["requestID"] = 123456789;
			//jsonpkt["header"]["type"] = static_cast<int>(ClientMsgType::REGISTER_UP);
			//jsonpkt["header"]["senderID"] = "123456";
			//jsonpkt["message"]["userID"] = "123456";
			//jsonpkt["message"]["userName"] = "Alex";
			//jsonpkt["message"]["password"] = "alex123456";
			//disp->dispatch(jsonpkt);

			//receive
			while (true) {
				//receive packet
				string str;
				int result = s->read(str);	
				//bool result = server->receive(s, );
				if (result == 2) {
					//server->disConnect;
					cout << "disconnected..." << endl;
					//TODO change to temp id
				}
				else if (result == 0) {
						cout << "receive error.." << endl;
				}

				Json::Value pkt = server->deserialize(str);

				//get checkSum
				int checkSum = pkt["checkSum"].asInt();
				pkt.removeMember("checkSum");

				//recalculate checkSum
				Json::FastWriter w;
				string tmp_str = w.write(pkt);
				// remove '\n' at the end of string
				if (!tmp_str.empty() && tmp_str.back() == '\n') {
					tmp_str.pop_back();
				}

				int sum = 0;
				for (auto i : tmp_str) {
					sum += i;
				}
				if (sum == checkSum) {
					cout << "checkSum correct!" << endl;	//test ************
				}
				else {
					cout << "checkSum incorrect.." << endl;	//test ************
					continue;
				}

				//add tmperary sender id for register and login request
				ClientMsgType type = static_cast<ClientMsgType>(pkt["header"]["type"].asInt());
				if (type == ClientMsgType::REGISTER_UP || type == ClientMsgType::LOG_IN) {
					pkt["header"]["senderID"] = tmpID;
				}
				
				Json::StyledWriter sw;													//test ********************
				cout << endl << "server receive: " << endl << sw.write(pkt) << endl;	//test ********************
				disp->dispatch(pkt);	//add task to thread pool
			}
		});
		myThread.detach();
	}
	
	acceptor.close();
	delete disp;
	delete tp;

	//this_thread::sleep_for(chrono::seconds(100));
}