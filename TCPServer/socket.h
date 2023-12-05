#pragma once
#include <iostream>
#include <mutex>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

class Socket
{
public:
	Socket(asio::io_context* io);
	~Socket();

	tcp::socket* get();
	/// <summary>
	/// read data from socket
	/// </summary>
	/// <param name="pkt">the buffer to contain output data</param>
	/// <returns> 1: run successful. 0: failed to read. 2: socket disconnected</returns>
	int read(vector<unsigned char>& pkt);
	void write(vector<unsigned char>& pkt);
	string getRemoteAddress();
	void close();
	void shutdown();
private:
	tcp::socket* m_socket;
	mutex m_mutex;
};

