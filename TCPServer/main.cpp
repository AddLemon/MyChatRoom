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
	SodiumEncryptor* encryptor = server->getEncryptor();
	vector<unsigned char> pubKey = encryptor->getPublicKey();

	//create temperary id
	int tmp = 100000;
	while (true) {
		tmp++;
		//accept connection
		Socket* s = acceptor.createSocket();
		acceptor.accept(s);
		cout << "Address: " << s->getRemoteAddress() << " connected!" << endl;
		
		// 1.输出服务器公钥，用于客户端加密其对称密钥
		s->write(pubKey);

		// 将公钥转换为16进制字符串并打印公钥
		std::cout << "STEP 1 - Send Server Public Key: " << endl << encryptor->toHex(pubKey) << endl << endl; //test

		// 2.从客户端接收加密的对称密钥
		vector<unsigned char> encrypted_symmetric_key_vec; // 从客户端接收
		s->read(encrypted_symmetric_key_vec);
		std::cout << "STEP 5 - Received Encrypted Symmetric Key: " << endl << encryptor->toHex(encrypted_symmetric_key_vec) << std::endl << endl;    //test

		// 将加密的对称密钥储存到char数组
		unsigned char encrypted_symmetric_key[crypto_box_SEALBYTES + crypto_secretbox_KEYBYTES];
		copy(encrypted_symmetric_key_vec.begin(), encrypted_symmetric_key_vec.end(), encrypted_symmetric_key);

		// 3.将加密的对称密钥解密
		unsigned char symmetric_key[crypto_secretbox_KEYBYTES];
		encryptor->decrypt_symmetric_key(encrypted_symmetric_key, symmetric_key, sizeof(encrypted_symmetric_key));
		std::cout << "STEP 6 - Decrypted Symmetric Key: " << endl << encryptor->toHex(symmetric_key, sizeof(symmetric_key)) << std::endl << endl;    //test


		//sub thread
		thread myThread([&, tmp, s, symmetric_key]() {
			string tmpID = to_string(tmp);
			server->newConnect(tmpID, s, symmetric_key);	//record sender id with socket pointer
			
			//receive
			while (true) {
				//receive packet
				Json::Value pkt;
				bool result = server->receive(s, pkt, symmetric_key);
				if (result != true) {
					//server->disConnect;
					cout << "Address: " << s->getRemoteAddress() << " disconnected..." << endl;
					break;
				}
				cout << "111111" << endl;	//test
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
				else if (type == ClientMsgType::LOG_OFF) {
					string senderID = pkt["header"]["senderID"].asString();
					server->renewSocketMapKey(senderID, tmpID);
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