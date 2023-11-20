#ifndef SENDCOMMANDS_H
#define SENDCOMMANDS_H

#include "client.h"
#include "public.h"

#include <QJsonObject>
#include <QRunnable>
#include <QObject>



class AbstractRequest : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit AbstractRequest(QObject *parent = nullptr);
    virtual ~AbstractRequest(){};
public:
    virtual void addHeader();
    void addCheckSum();
    void send();
signals:
    void readySend(QByteArray pkt);
protected:
    Client* client;
    int requestID = 123456;
    MsgType type;
    QString senderID;
    QJsonObject pkt;
};

class RegisterCommand : public AbstractRequest
{
public:
    RegisterCommand(QString& userID, QString& userName, QString& password, QObject *parent = nullptr)
        : AbstractRequest(parent), m_id(userID), m_name(userName), m_password(password)
    {
        type = MsgType::REGISTER_UP;
    };
public:
    void run() override;

private:
    QString m_id;
    QString m_name;
    QString m_password;
};

class logInCommand : public AbstractRequest
{
public:
    logInCommand(QString& userID, QString& password, QObject *parent = nullptr)
        : AbstractRequest(parent), m_id(userID), m_password(password)
    {
        type = MsgType::LOG_IN;
        senderID = "";
    };
public:
    void run() override;
private:
    QString m_id;
    QString m_password;
};

class mdfyUsrInfoCommand : public AbstractRequest
{
public:
    mdfyUsrInfoCommand(QString& userName, QString& password, QObject *parent = nullptr)
        : AbstractRequest(parent), m_name(userName), m_password(password)
    {
        type = MsgType::MODIFY_USER_INFO;
    };
public:
    void run() override;
private:
    QString m_name;
    QString m_password;
};

class usrCommand : public AbstractRequest
{
public:
    usrCommand(QString& userID, QObject *parent = nullptr)
        : AbstractRequest(parent), m_id(userID){};
public:
    void run() override;
protected:
    QString m_id;
};

class findUsrCommand : public usrCommand
{
public:
    findUsrCommand(QString& userID, QObject *parent = nullptr)
        : usrCommand(userID, parent)
    {
        type = MsgType::FIND_USER;
    };
};

class addFrndCommand : public usrCommand
{
public:
    addFrndCommand(QString& userID, QObject *parent = nullptr)
        : usrCommand(userID, parent)
    {
        type = MsgType::ADD_FRIEND;
    };
};

class rmvFrndCommand : public usrCommand
{
public:
    rmvFrndCommand(QString& userID, QObject *parent = nullptr)
        : usrCommand(userID, parent)
    {
        type = MsgType::REMOVE_FRIEND;
    };
};

class grpCommand : public AbstractRequest
{
public:
    grpCommand(int id, QObject *parent = nullptr)
        : AbstractRequest(parent), m_groupID(id){};
public:
    void run() override;
protected:
    int m_groupID;
};

class findGrpCommand : public grpCommand
{
public:
    findGrpCommand(int id, QObject *parent = nullptr)
        : grpCommand(id, parent)
    {
        type = MsgType::FIND_GROUP;
    };
};

class addGrpCommand : public grpCommand
{
public:
    addGrpCommand(int id, QObject *parent = nullptr)
        : grpCommand(id, parent)
    {
        type = MsgType::ADD_GROUP;
    };
};

class rmvGrpCommand : public grpCommand
{
public:
    rmvGrpCommand(int id, QObject *parent = nullptr)
        : grpCommand(id, parent)
    {
        type = MsgType::REMOVE_GROUP;
    };
};

class getGrpMbrCommand : public grpCommand
{
public:
    getGrpMbrCommand(int id, QObject *parent = nullptr)
        : grpCommand(id, parent)
    {
        type = MsgType::GET_GROUP_MEMBERS;
    }
};

class crtGrpCommand : public AbstractRequest
{
public:
    crtGrpCommand(QString& name, QObject *parent = nullptr)
        : AbstractRequest(parent), m_groupName(name)
    {
        type = MsgType::CREATE_GROUP;
    };
public:
    void run() override;
private:
    QString m_groupName;
};

class mdfyGrpInfoCommand : public AbstractRequest
{
public:
    mdfyGrpInfoCommand(QString& name, QObject *parent = nullptr)
        : AbstractRequest(parent), m_groupName(name)
    {
        type = MsgType::MODIFY_GROUP_INFO;
    };
public:
    void run() override;
private:
    QString m_groupName;
};

class chatCommand : public AbstractRequest
{
public:
    chatCommand(QString& content, QString& time, QObject *parent = nullptr)
        : AbstractRequest(parent), m_content(content), m_timestamp(time){};
public:
    void run() override;
protected:
    QString m_content;
    QString m_timestamp;
};

class pvChatCommand : public chatCommand
{
public:
    pvChatCommand(QString& receiverID, QString& content, QString& time, QObject *parent = nullptr)
        : chatCommand(content, time, parent), m_receiverID(receiverID)
    {
        type = MsgType::PRIVATE_CHAT;
    };
public:
    void addHeader() override;
private:
    QString m_receiverID;
};

class gpChatCommand : public chatCommand
{
public:
    gpChatCommand(int groupID, QString& content, QString& time, QObject *parent = nullptr)
        : chatCommand(content, time, parent), m_groupID(groupID)
    {
        type = MsgType::GROUP_CHAT;
    };
public:
    void addHeader() override;
private:
    int m_groupID;
};



#endif // SENDCOMMANDS_H
