#include "recvcommands.h"

#if 1   //test
AbstractReception::AbstractReception(QJsonObject pkt, QObject *parent)
    : QObject(parent), QRunnable()
{
    setAutoDelete(true);

    m_client = Client::getInstance();
    m_header = pkt["header"].toObject();
    m_message = pkt["message"].toObject();
    m_type = static_cast<RecvType>(m_header["type"].toInt());
}

ReplyPrcsCommand::ReplyPrcsCommand(QJsonObject pkt, QObject *parent)
    : AbstractReception(pkt, parent)
{
    m_requestID = m_header["requestID"].toInt();
    m_status = m_message["status"].toBool();
};

ReplyRegisterCommand::ReplyRegisterCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status != true){
        m_reason = m_message["reason"].toString();
    }
}

void ReplyRegisterCommand::run()
{
    if (m_status == true) {
        emit m_client->registerSuccessful();
    } else {
        emit m_client->registerFailed(m_reason);
    }
};

ReplyLogInCommand::ReplyLogInCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        QJsonObject userInfo = data["userInfo"].toObject();
        QJsonArray friends = data["friends"].toArray();
        QJsonArray groups = data["groups"].toArray();

        //提取用户个人信息数据
        m_selfInfo.userID = userInfo["userID"].toString();
        m_selfInfo.userName = userInfo["userName"].toString();
        m_selfInfo.password = userInfo["password"].toString();

        for (const QJsonValue &value : friends) {
            //遍历friendsVector提取好友列表数据
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                User user;
                user.id = obj["userID"].toString();
                user.name = obj["userName"].toString();
                user.status = obj["status"].toBool();

                // 将键值对添加到 QVector
                m_friends.append(user);
            }
        }

        for (const QJsonValue &value : groups) {
            //遍历groupsVector提取群聊列表数据
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                Group group;
                group.id = obj["groupID"].toInt();
                group.name = obj["groupName"].toString();

                // 将键值对添加到 QVector
                m_groups.append(group);
            }
        }
    }
    else {
        m_reason = m_message["reason"].toString();
    }

    //登录成功后，客户端处理接收的数据
    connect(this, &ReplyLogInCommand::loginSuccessful, m_client, &Client::onLogIn);
}

void ReplyLogInCommand::run()
{
    if (m_status == true) {
        emit loginSuccessful(m_selfInfo, m_friends, m_groups);
    } else {
        emit m_client->loginFailed(m_reason);
    }
}

ReplyMdfyUsrInfoCommand::ReplyMdfyUsrInfoCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status != true){
        m_reason = m_message["reason"].toString();
    }
}

void ReplyMdfyUsrInfoCommand::run()
{
    if (m_status == true) {
        emit m_client->settingSuccessful();
    } else {
        emit m_client->settingFailed(m_reason);
    }
}

ReplyFindUsrCommand::ReplyFindUsrCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_userID = data["userID"].toString();
        m_userName = data["userName"].toString();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
}

void ReplyFindUsrCommand::run()
{
    if (m_status == true) {
        emit m_client->findUserSuccessful(m_userID, m_userName);
    } else {
        emit m_client->findUserFailed(m_reason);
    }
}

ReplyAddFrndCommand::ReplyAddFrndCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_userID = data["userID"].toString();
        m_userName = data["userName"].toString();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
    //添加成功后，客户端将接收的数据并写入model
    connect(this, &ReplyAddFrndCommand::addFriendSuccessful, m_client, &Client::onNewFriend);
}

void ReplyAddFrndCommand::run()
{
    if (m_status == true) {
        emit addFriendSuccessful(m_userID, m_userName);
    } else {
        emit m_client->addFriendFailed(m_reason);
    }
}

ReplyRmvFrndCommand::ReplyRmvFrndCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_userID = data["userID"].toString();
    }
    else{
        m_reason = m_message["reason"].toString();
    }
    //删除成功后，客户端将接收的数据并写入model
    connect(this, &ReplyRmvFrndCommand::rmvFriendSuccessful, m_client, &Client::onRemoveFriend);
}

void ReplyRmvFrndCommand::run()
{
    if (m_status == true) {
        emit rmvFriendSuccessful(m_userID);
    } else {
        emit m_client->rmvFriendFailed(m_reason);
    }
}

ReplyFindGrpCommand::ReplyFindGrpCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_groupID = data["groupID"].toInt();
        m_groupName = data["groupName"].toString();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
}

void ReplyFindGrpCommand::run()
{
    if (m_status == true) {
        emit m_client->findGroupSuccessful(m_groupID, m_groupName);
    } else {
        emit m_client->findGroupFailed(m_reason);
    }
}

ReplyAddGrpCommand::ReplyAddGrpCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_groupID = data["groupID"].toInt();
        m_groupName = data["groupName"].toString();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
    //添加成功后，客户端将接收的数据并写入model
    connect(this, &ReplyAddGrpCommand::addGroupSuccessful, m_client, &Client::onNewGroup);
}

void ReplyAddGrpCommand::run()
{
    if (m_status == true) {
        emit addGroupSuccessful(m_groupID, m_groupName);
    } else {
        emit m_client->addGroupFailed(m_reason);
    }
}

ReplyRmvGrpCommand::ReplyRmvGrpCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_groupID = data["groupID"].toInt();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
    //删除成功后，客户端将接收的数据并写入model
    connect(this, &ReplyRmvGrpCommand::rmvGroupSuccessful, m_client, &Client::onRemoveGroup);
}

void ReplyRmvGrpCommand::run()
{
    if (m_status == true) {
        emit rmvGroupSuccessful(m_groupID);
    } else {
        emit m_client->rmvGroupFailed(m_reason);
    }
}

ReplyGetGrpMbrCommand::ReplyGetGrpMbrCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        QJsonArray members = data["members"].toArray();

        //提取群id数据
        m_groupID = data["groupID"].toInt();

        for (const QJsonValue &value : members) {
            //遍历friendsVector提取群成员列表数据
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                User user;
                user.id = obj["userID"].toString();
                user.name = obj["userName"].toString();

                // 将键值对添加到 QVector
                m_members.append(user);
            }
        }
    }
    else {
        m_reason = m_message["reason"].toString();
    }
    //获取成功后，客户端将接收的数据并写入model
    connect(this, &ReplyGetGrpMbrCommand::getMembersSuccessful, m_client, &Client::onNewMembers);
}

void ReplyGetGrpMbrCommand::run()
{
    if (m_status == true) {
        emit getMembersSuccessful(m_groupID, m_members);
    } else {
        emit m_client->getMembersFailed(m_reason);
    }
}

ReplyCrtGrpCommand::ReplyCrtGrpCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status == true){
        QJsonObject data = m_message["data"].toObject();
        m_groupID = data["groupID"].toInt();
        m_groupName = data["groupName"].toString();
    }
    else {
        m_reason = m_message["reason"].toString();
    }
    //获取成功后，客户端将接收的数据并写入model
    connect(this, &ReplyCrtGrpCommand::crtGroupSuccessful, m_client, &Client::onNewGroup);
}

void ReplyCrtGrpCommand::run()
{
    if (m_status == true) {
        emit crtGroupSuccessful(m_groupID, m_groupName);
        emit m_client->crtGroupSuccessful(m_groupID, m_groupName);
    } else {
        emit m_client->crtGroupFailed(m_reason);
    }
}

ReplyMdfyGrpInfoCommand::ReplyMdfyGrpInfoCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status != true){
        m_reason = m_message["reason"].toString();
    }
}

void ReplyMdfyGrpInfoCommand::run()
{
    if (m_status == true) {
        emit m_client->mdfyGroupSuccessful();
    } else {
        emit m_client->mdfyGroupFailed(m_reason);
    }
}

ReplyPvChatCommand::ReplyPvChatCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status != true){
        m_reason = m_message["reason"].toString();
    }
}

void ReplyPvChatCommand::run()
{
    if (m_status == true) {
        emit m_client->pvChatSuccessful();
    } else {
        emit m_client->pvChatFailed(m_reason);
    }
}

ReplyGpChatCommand::ReplyGpChatCommand(QJsonObject pkt, QObject *parent)
    : ReplyPrcsCommand(pkt, parent)
{
    if(m_status != true){
        m_reason = m_message["reason"].toString();
    }
}

void ReplyGpChatCommand::run()
{
    if (m_status == true) {
        emit m_client->gpChatSuccessful();
    } else {
        emit m_client->gpChatFailed(m_reason);
    }
}

NoticePrcsCommand::NoticePrcsCommand(QJsonObject pkt, QObject *parent)
    : AbstractReception(pkt, parent)
{
    m_id = m_message["userID"].toString();
    m_name = m_message["userName"].toString();
    //收到添加好友消息，客户端将接收的数据并写入model
    connect(this, &NoticePrcsCommand::newFriend, m_client, &Client::onNewFriend);
    //收到删除好友消息，客户端将接收的数据并写入model
    connect(this, &NoticePrcsCommand::unfriended, m_client, &Client::onRemoveFriend);
}

void NoticePrcsCommand::run()
{
    if(m_type == RecvType::SERVER_NOTICE_NEW_FRIEND){
        emit newFriend(m_id, m_name);
    }
    else if(m_type == RecvType::SERVER_NOTICE_REMOVE_FRIEND){
        emit unfriended(m_id);
    }
}

NoticeStatusCommand::NoticeStatusCommand(QJsonObject pkt, QObject *parent)
    : AbstractReception(pkt, parent)
{
    m_id = m_message["userID"].toString();
    m_status = m_message["status"].toBool();
    //收到好友的新状态，客户端将接收的数据并写入model
    connect(this, &NoticeStatusCommand::newStatus, m_client, &Client::onNewStatus);
}

void NoticeStatusCommand::run()
{
    emit newStatus(m_id, m_status);
}

ForwardPrcsCommand::ForwardPrcsCommand(QJsonObject pkt, QObject *parent)
    : AbstractReception(pkt, parent)
{
    m_senderID = m_header["senderID"].toString();
    m_groupID = m_header["groupID"].toInt();
    m_content = m_message["content"].toString();
    m_timestamp = m_message["timestamp"].toString();

    //收到私聊消息，客户端将接收的数据并写入recvManager
    connect(this, &ForwardPrcsCommand::newPrivateMsg, m_client, &Client::onNewPrivateMsg);
    //收到群聊消息，客户端将接收的数据并写入recvManager
    connect(this, &ForwardPrcsCommand::newGroupMsg, m_client, &Client::onNewGroupMsg);
}

void ForwardPrcsCommand::run()
{
    if(m_type == RecvType::SERVER_FORWARD_PRIVATE_CHAT){
        Message msg = {m_senderID, m_content, m_timestamp};
        emit newPrivateMsg(m_senderID, msg);
    }
    else if(m_type == RecvType::SERVER_FORWARD_GROUP_CHAT){
        Message msg = {m_senderID, m_content, m_timestamp};
        emit newGroupMsg(m_groupID, msg);
    }
}
#endif //test


