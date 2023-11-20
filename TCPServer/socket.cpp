#include "socket.h"
#include "json/json.h"

Socket::Socket(asio::io_context* io)	//tcp::socket* socket
{
	m_socket = new tcp::socket(*io);
}

Socket::~Socket()
{
	shutdown();
	delete m_socket;
}

tcp::socket* Socket::get()
{
	return m_socket;
}

int Socket::read(string& pkt)
{
	//receive data and resize the buffer
	vector<char> buffer(1024);
	size_t size;
	system::error_code error;

	cout << "start to receive..." << endl;	//test ***********************
	
	size = m_socket->read_some(asio::buffer(buffer), error);	//读取
	if (error == asio::error::eof) {
		// 对方正常关闭连接
		return 2;
	}
	else if (error) {
		// 发生其他错误
		cout << "Read error: " << error << " - " << error.message() << endl;
		return 0;
	}

	pkt = string(buffer.begin(), buffer.begin() + size);

	return 1;
}

void Socket::write(string pkt)
{
	//lock
	unique_lock<mutex> lock(m_mutex);
	cout << "send string:" << endl << pkt << endl;	//test ********************
	//send packet
	system::error_code error;
	//asio::write(m_socket, asio::buffer(pkt), error);
	size_t size = m_socket->send(asio::buffer(pkt), 0, error);
	cout << "send size: " << size << endl;
	if (error) {
		cout << "Send error: " << error << " - " << error.message() << endl;
	}
}

string Socket::getRemoteAddress()
{
	tcp::endpoint remote_ep = m_socket->remote_endpoint();
	asio::ip::address remote_ad = remote_ep.address();
	return remote_ad.to_string();
}

void Socket::close()
{
	system::error_code error;
	m_socket->close(error);
	if (error) {
		cout << "Error: " << error << " - " << error.message() << endl;
	}
}

void Socket::shutdown()
{
	system::error_code error;
	m_socket->shutdown(tcp::socket::shutdown_both, error);
	if (error) {
		cout << "Error: " << error << " - " << error.message() << endl;
	}
}
