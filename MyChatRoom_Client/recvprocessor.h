#ifndef RECVPROCESSOR_H
#define RECVPROCESSOR_H

#include "client.h"

#include <QByteArray>
#include <QObject>
#if 1   //test
class RecvProcessor : public QObject
{
    Q_OBJECT
public:
    explicit RecvProcessor(QObject *parent = nullptr);
public slots:
    void process(QByteArray pkt);

private:
    bool checkCheckSum(QJsonObject& pkt);

signals:

private:
    Client* m_client;
    //QJsonObject m_pkt;
};
#endif //test
#endif // RECVPROCESSOR_H
