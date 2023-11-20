#include "client.h"

#include <QJsonDocument>

Client* Client::m_obj = new Client;

Client::Client(QObject *parent): QObject(parent)
{
    init();
}

Client::~Client()
{
}

void Client::init()
{
    m_self = new Self(this);
    m_friendsModel = new FriendsModel(this);
    m_groupsModel = new GroupsModel(this);
}

void Client::restart()
{
    m_self->deleteLater();
    m_friendsModel->deleteLater();
    m_groupsModel->deleteLater();
    qDeleteAll(m_membersModelMap);
    m_membersModelMap.clear();
    init();
}

QByteArray Client::serialize(QJsonObject &pkt)
{
    QJsonDocument doc = QJsonDocument(pkt);
    QByteArray result = doc.toJson(QJsonDocument::Compact);   //serialize into text format
    return result;
}

QJsonObject Client::deserialize(QByteArray &pkt)
{
    QJsonDocument doc = QJsonDocument::fromJson(pkt);
    return doc.object();
}

QJsonObject Client::packRegisterInfo(QString &userID, QString &userName, QString &password)
{
    QJsonObject msg = QJsonObject();
    msg.insert("userID", userID);
    msg.insert("userName", userName);
    msg.insert("password", password);
    return msg;
}

QJsonObject Client::packLogInInfo(QString &userID, QString &password)
{
    QJsonObject msg = QJsonObject();
    msg.insert("userID", userID);
    msg.insert("password", password);
    return msg;
}

QJsonObject Client::packRenewInfo(QString& userName, QString& password)
{
    QJsonObject msg = QJsonObject();
    if(!userName.isEmpty()){
        msg.insert("userName", userName);
    }
    if(!password.isEmpty()){
        msg.insert("password", password);
    }
    return msg;
}

QJsonObject Client::packID(QString &userID)
{
    QJsonObject msg = QJsonObject();
    msg.insert("userID", userID);
    return msg;
}

QJsonObject Client::packID(int groupID)
{
    QJsonObject msg = QJsonObject();
    msg.insert("groupID", groupID);
    return msg;
}

QJsonObject Client::packNewGroupInfo(QString &groupName)
{
    QJsonObject msg = QJsonObject();
    msg.insert("groupName", groupName);
    return msg;
}

QJsonObject Client::packRenewGroupInfo(QString& groupName)
{
    QJsonObject msg = QJsonObject();
    if(!groupName.isEmpty()){
        msg.insert("groupName", groupName);
    }
    return msg;
}

QJsonObject Client::packChatInfo(QString &content, QString &timestamp)
{
    QJsonObject msg = QJsonObject();
    msg.insert("content", content);
    msg.insert("timestamp", timestamp);
    return msg;
}

void Client::onLogIn(SelfInfo userInfo, QList<User> friends, QList<Group> groups)
{
    m_self->init(userInfo.userID, userInfo.userName, userInfo.password);

    for(QList<User>::iterator i = friends.begin(); i != friends.end(); ++i){
        m_friendsModel->add(i->id, i->name);
    }

    for(QList<Group>::iterator i = groups.begin(); i != groups.end(); ++i){
        m_groupsModel->add(i->id, i->name);
    }
    emit loginSuccessful();
}

void Client::onNewFriend(QString id, QString name)
{
    m_friendsModel->add(id, name);
    emit addFriendSuccessful();
}

void Client::onRemoveFriend(QString id)
{
    m_friendsModel->remove(id);
    emit rmvFriendSuccessful();
}

void Client::onNewGroup(int id, QString name)
{
    m_groupsModel->add(id, name);
    emit addGroupSuccessful();
}

void Client::onRemoveGroup(int id)
{
    m_groupsModel->remove(id);
    emit rmvGroupSuccessful();
}

void Client::onNewMembers(int groupID, QList<User> members)
{
    if(m_membersModelMap.contains(groupID)){
        //已存在，则更新model
        MembersModel* mModel = m_membersModelMap.value(groupID);
        //mModel->init(); //test
        mModel->load(members);
        emit frshMembersSuccessful();   //发送信号通知窗口
    }
    else{
        //不存在，则新建model
        MembersModel* mModel = new MembersModel(this);
        //mModel->init(); //test
        mModel->load(members);
        m_membersModelMap.insert(groupID, mModel);  //记录创建的model
        emit getMembersSuccessful();    //发送信号通知窗口
    }
}






