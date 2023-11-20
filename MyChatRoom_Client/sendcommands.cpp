#include "sendcommands.h"

#include <QJsonDocument>

AbstractRequest::AbstractRequest(QObject *parent) : QObject(parent), QRunnable()
{
    setAutoDelete(true);

    client = Client::getInstance();
    senderID = client->m_self->getID();

    connect(this, &RegisterCommand::readySend, client, &Client::readySend);
}

void AbstractRequest::addHeader()
{
    QJsonObject header = QJsonObject();
    header.insert("requestID", requestID);
    header.insert("type", static_cast<int>(type));
    header.insert("senderID", senderID);
    pkt.insert("header", header);   //insert to send packet
}

void AbstractRequest::addCheckSum()
{
    int checkSum = 0;
    QJsonDocument doc = QJsonDocument(pkt);
    QString str = doc.toJson(QJsonDocument::Compact);
    for(QChar i : str){
        checkSum += static_cast<unsigned char>(i.toLatin1());
        //qDebug() << i << ": " << static_cast<unsigned char>(i.toLatin1());  //test *******
    }
    pkt.insert("checkSum", checkSum);   //insert to send packet
}

void AbstractRequest::send()
{
    QJsonDocument doc = QJsonDocument(pkt);
    QByteArray toSend = doc.toJson(QJsonDocument::Compact);
    //qDebug() << toSend;     //test ********************
    emit readySend(toSend);
}

void RegisterCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packRegisterInfo(m_id, m_name, m_password); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void logInCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packLogInInfo(m_id, m_password); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void mdfyUsrInfoCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packRenewInfo(m_name, m_password); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void usrCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packID(m_id); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void grpCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packID(m_groupID); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void crtGrpCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packNewGroupInfo(m_groupName); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void mdfyGrpInfoCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packRenewGroupInfo(m_groupName); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void chatCommand::run()
{
    QJsonObject message = QJsonObject();
    message = client->packChatInfo(m_content, m_timestamp); //pack message
    pkt.insert("message", message);   //insert to send packet
    addHeader();
    addCheckSum();
    send();
}

void pvChatCommand::addHeader()
{
    QJsonObject header = QJsonObject();
    header.insert("requestID", requestID);
    header.insert("type", static_cast<int>(type));
    header.insert("senderID", senderID);
    header.insert("receiverID", m_receiverID);
    pkt.insert("header", header);   //insert to send packet
}

void gpChatCommand::addHeader()
{
    QJsonObject header = QJsonObject();
    header.insert("requestID", requestID);
    header.insert("type", static_cast<int>(type));
    header.insert("senderID", senderID);
    header.insert("groupID", m_groupID);
    pkt.insert("header", header);   //insert to send packet
}
