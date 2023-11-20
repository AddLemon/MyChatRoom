#ifndef SOCKET_H
#define SOCKET_H

#include <QByteArray>
#include <QString>
#include <QTcpSocket>
#include <QObject>
#include <QMutex>



class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    void connectToServer(QString ip = "127.0.0.1", unsigned short port = 5000);
    void close();
    void write(QByteArray pkt);
    void read(QByteArray& pkt);

signals:
    void connected();
    void received(QByteArray pkt);
private:
    QTcpSocket* m_socket = nullptr;
};

#endif // SOCKET_H
