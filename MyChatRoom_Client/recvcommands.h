#ifndef RECVCOMMANDS_H
#define RECVCOMMANDS_H

#include "client.h"
#include "public.h"

#include <QJsonArray>
#include <QObject>
#include <QRunnable>

#if 1   //test
class AbstractReception : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit AbstractReception(QJsonObject pkt, QObject *parent = nullptr);

protected:
    Client* m_client;
    QJsonObject m_header;
    QJsonObject m_message;
    RecvType m_type;
};

class ReplyPrcsCommand : public AbstractReception
{
    Q_OBJECT
public:
    ReplyPrcsCommand(QJsonObject pkt, QObject *parent = nullptr);

protected:
    int m_requestID; // not used
    bool m_status;
    QString m_reason;
};

class ReplyRegisterCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyRegisterCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
};

class ReplyLogInCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyLogInCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void loginSuccessful(SelfInfo selfInfo,
                         QList<User> friends,
                         QList<Group> groups);
private:
    SelfInfo m_selfInfo;
    QList<User> m_friends;
    QList<Group> m_groups;
};

class ReplyMdfyUsrInfoCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyMdfyUsrInfoCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;

private:
};

class ReplyFindUsrCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyFindUsrCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
    QString m_userID;
    QString m_userName;
};

class ReplyAddFrndCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyAddFrndCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void addFriendSuccessful(QString id, QString name);
private:
    QString m_userID;
    QString m_userName;
};

class ReplyRmvFrndCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyRmvFrndCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void rmvFriendSuccessful(QString userID);
private:
    QString m_userID;
};

class ReplyFindGrpCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyFindGrpCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
    int m_groupID;
    QString m_groupName;
};

class ReplyAddGrpCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyAddGrpCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void addGroupSuccessful(int id, QString name);
private:
    int m_groupID;
    QString m_groupName;
};

class ReplyRmvGrpCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyRmvGrpCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void rmvGroupSuccessful(int groupID);
private:
    int m_groupID;
};

class ReplyGetGrpMbrCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyGetGrpMbrCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void getMembersSuccessful(int groupID, QList<User> members);
private:
    int m_groupID;
    QList<User> m_members;
};

class ReplyCrtGrpCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyCrtGrpCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void crtGroupSuccessful(int groupID, QString groupName);
private:
    int m_groupID;
    QString m_groupName;
};

class ReplyMdfyGrpInfoCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyMdfyGrpInfoCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
};

class ReplyPvChatCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyPvChatCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
};

class ReplyGpChatCommand : public ReplyPrcsCommand
{
    Q_OBJECT
public:
    ReplyGpChatCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
private:
};

class NoticePrcsCommand : public AbstractReception
{
    Q_OBJECT
public:
    NoticePrcsCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void newFriend(QString id, QString name);
    void unfriended(QString id);
private:
    QString m_id;
    QString m_name;
};

class ForwardPrcsCommand : public AbstractReception
{
    Q_OBJECT
public:
    ForwardPrcsCommand(QJsonObject pkt, QObject *parent = nullptr);
    void run() override;
signals:
    void newPrivateMsg(QString senderID, Message msg);
    void newGroupMsg(int groupID, Message msg);
private:
    QString m_senderID;
    int m_groupID;
    QString m_content;
    QString m_timestamp;
};
#endif  //test
#endif // RECVCOMMANDS_H
