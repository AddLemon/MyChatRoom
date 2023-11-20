#include "socket.h"

#include <QHostAddress>

Socket::Socket(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, [=](){
        emit connected();
        qDebug() << "Socket coonect..";     //test
    });
    connect(m_socket, &QTcpSocket::readyRead, this, [=](){
        qDebug() << "Socket ready read..";      //test
        QByteArray pkt;
        read(pkt);
        emit received(pkt);
    });
    connect(m_socket, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError socketError){
        qDebug() << "err: " << socketError;
    });
    connectToServer();
}

void Socket::connectToServer(QString ip, unsigned short port)
{
    m_socket->connectToHost(QHostAddress(ip), port);
    m_socket->waitForConnected();
}

void Socket::close()
{
    m_socket->disconnectFromHost();
    m_socket->deleteLater();
    m_socket = nullptr;
}

void Socket::write(QByteArray pkt)
{
    if(m_socket->waitForConnected()){
        m_socket->write(pkt);
        m_socket->flush();
        m_socket->waitForBytesWritten();
    }
}

void Socket::read(QByteArray &pkt)
{
    pkt = m_socket->readAll();

}
