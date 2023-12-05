#ifndef SOCKET_H
#define SOCKET_H

#include <QByteArray>
#include <QString>
#include <QTcpSocket>
#include <QObject>
#include <QMutex>
#include "sodiumencryptor.h"



class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    void connectToServer(QString ip = "127.0.0.1", unsigned short port = 5000);
    void close();
    void write(QByteArray pkt);
    void read(QByteArray& pkt);
    bool isOpen();
signals:
    void connected();
    void disconnected();
    void received(QByteArray pkt);
    void failInitEncry();
private:
    QTcpSocket* m_socket = nullptr;
    SodiumEncryptor* m_encryptor;

};

#endif // SOCKET_H
