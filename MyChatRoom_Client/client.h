#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QByteArray>
#include <QByteArray>
#include <QJsonObject>
#include <QList>
#include <QStandardItemModel>
#include "membersmodel.h"
#include "socket.h"
#include "self.h"
#include "friendsmodel.h"
#include "groupsmodel.h"

class Client : public QObject
{
    Q_OBJECT
public:
    Client(const Client& client) = delete;
    Client& operator=(const Client& client) = delete;
    static Client* getInstance(){
        return m_obj;
    }
private:
    explicit Client(QObject *parent = nullptr);
    ~Client();


public:
    void init();
    void restart();
    QByteArray serialize(QJsonObject& pkt);
    QJsonObject deserialize(QByteArray& pkt);

    //process sending
    QJsonObject packRegisterInfo(QString& userID, QString& userName, QString& password);
    QJsonObject packLogInInfo(QString& userID, QString& password);
    QJsonObject packRenewInfo(QString& userName, QString& password);
    QJsonObject packID(QString& userID);
    QJsonObject packID(int groupID);
    QJsonObject packNewGroupInfo(QString& groupName);
    QJsonObject packRenewGroupInfo(QString& groupName);
    QJsonObject packChatInfo(QString& content, QString& timestamp);

    //process reception/models
    void onLogIn(SelfInfo selfInfo,
                 QList<User> friends,
                 QList<Group> groups);
    void onNewFriend(QString id, QString name);
    void onRemoveFriend(QString id);
    void onNewGroup(int id, QString name);
    void onRemoveGroup(int id);
    void onNewMembers(int groupID, QList<User> members);    //接收到成员列表后调用此方法将数据更新到model

//    size_t getSizeFdList();
//    size_t getSizeGpList();

signals:
    //send
    void readySend(QByteArray pkt);

    //recv
    void registerSuccessful();
    void registerFailed(QString reason);
    void loginSuccessful();//
    void loginFailed(QString reason);
    void settingSuccessful();
    void settingFailed(QString reason);
    void findUserSuccessful(QString id, QString name);//
    void findUserFailed(QString reason);//
    void addFriendSuccessful();//
    void addFriendFailed(QString reason);//
    void rmvFriendSuccessful();//
    void rmvFriendFailed(QString reason);//
    void findGroupSuccessful(int id, QString name);//
    void findGroupFailed(QString reason);//
    void addGroupSuccessful();//
    void addGroupFailed(QString reason);//
    void rmvGroupSuccessful();//
    void rmvGroupFailed(QString reason);//
    void getMembersSuccessful();//
    void getMembersFailed(QString reason);
    void frshMembersSuccessful();//
    void crtGroupSuccessful(int id, QString name);//
    void crtGroupFailed(QString reason);//
    void mdfyGroupSuccessful();
    void mdfyGroupFailed(QString reason);
    void pvChatSuccessful();
    void pvChatFailed(QString reason);
    void gpChatSuccessful();
    void gpChatFailed(QString reason);

//    //notice
//    void newFriend();
//    void removeFriend();

    //forward
    void newPrivateMsg(QString friendID, Message msg);
    void newGroupMsg(int groupID, Message msg);



public:
    //data and model
    Self* m_self;
    FriendsModel *m_friendsModel;
    GroupsModel *m_groupsModel;
    QMap<int, MembersModel*> m_membersModelMap;

private:
    static Client* m_obj;
    //QMutex* m_mutex;


};

#endif // CLIENT_H
