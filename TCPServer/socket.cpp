#include "socket.h"
#include "json/json.h"

Socket::Socket(asio::io_context* io)	//tcp::socket* socket
{
	m_socket = new tcp::socket(*io);
	//m_socket = socket;
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
		cout << "Error: " << error << " - " << error.message() << endl;
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
	try {
		m_socket->send(asio::buffer(pkt));
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
	}
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
