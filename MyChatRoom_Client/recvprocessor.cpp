#include "recvprocessor.h"
#include "recvcommands.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QThreadPool>

#if 1   //test

RecvProcessor::RecvProcessor(QObject *parent)
    : QObject{parent}
{
    m_client = Client::getInstance();
}

bool RecvProcessor::checkCheckSum(QJsonObject& pkt)
{
    int sum = 0;
    int checkSum = pkt["checkSum"].toInt();
    pkt.remove("checkSum");
    QJsonDocument doc = QJsonDocument(pkt);
    QString str = doc.toJson(QJsonDocument::Compact);
    for(QChar i : str){
        sum += static_cast<unsigned char>(i.toLatin1());
    }
    if(checkSum == sum){
        return true;
    }
    return false;
}

void RecvProcessor::process(QByteArray pkt)
{
    QJsonObject pkt_obj = m_client->deserialize(pkt);
    QJsonDocument doc = QJsonDocument(pkt_obj);         //test
    QString str = doc.toJson(QJsonDocument::Indented);  //test     QJsonDocument::Compact
    qDebug() << "Client receive: ";                     //test
    qDebug() << str;                                    //test

    if(!checkCheckSum(pkt_obj)){
        qDebug() << "checkSum incorrect..";
        return;
    }
    else{
        qDebug() << "checkSum correct!";
    }

    //根据type分解数据并创建相应的任务对象
    QJsonObject header = pkt_obj["header"].toObject();
    RecvType type = static_cast<RecvType>(header["type"].toInt());
    QThreadPool* tpool = QThreadPool::globalInstance();

    //创建任务对象并加入线程池
    switch(type)
    {
    case RecvType::SERVER_REPLY_REGISTER_UP:
    {
        ReplyRegisterCommand* notice = new ReplyRegisterCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_LOG_IN:
    {
        ReplyLogInCommand* notice = new ReplyLogInCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_MODIFY_USER_INFO:
    {
        ReplyMdfyUsrInfoCommand* notice = new ReplyMdfyUsrInfoCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_FIND_USER:
    {
        ReplyFindUsrCommand* notice = new ReplyFindUsrCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_ADD_FRIEND:
    {
        ReplyAddFrndCommand* notice = new ReplyAddFrndCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_REMOVE_FRIEND:
    {
        ReplyRmvFrndCommand* notice = new ReplyRmvFrndCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_FIND_GROUP:
    {
        ReplyFindGrpCommand* notice = new ReplyFindGrpCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_ADD_GROUP:
    {
        ReplyAddGrpCommand* notice = new ReplyAddGrpCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_REMOVE_GROUP:
    {
        ReplyRmvGrpCommand* notice = new ReplyRmvGrpCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_GET_GROUP_MEMBERS:
    {
        ReplyGetGrpMbrCommand* notice = new ReplyGetGrpMbrCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_CREATE_GROUP:
    {
        ReplyCrtGrpCommand* notice = new ReplyCrtGrpCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_MODIFY_GROUP_INFO:
    {
        ReplyMdfyGrpInfoCommand* notice = new ReplyMdfyGrpInfoCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_PRIVATE_CHAT:
    {
        ReplyPvChatCommand* notice = new ReplyPvChatCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_REPLY_GROUP_CHAT:
    {
        ReplyGpChatCommand* notice = new ReplyGpChatCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_NOTICE_NEW_FRIEND:
    {
        NoticePrcsCommand* notice = new NoticePrcsCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_NOTICE_REMOVE_FRIEND:
    {
        NoticePrcsCommand* notice = new NoticePrcsCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_FORWARD_PRIVATE_CHAT:
    {
        ForwardPrcsCommand* notice = new ForwardPrcsCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    case RecvType::SERVER_FORWARD_GROUP_CHAT:
    {
        ForwardPrcsCommand* notice = new ForwardPrcsCommand(pkt_obj);
        tpool->start(notice);
        break;
    }
    default:
        break;
    }















//    if(type == RecvType::SERVER_NOTICE_NEW_FRIEND || type == RecvType::SERVER_NOTICE_REMOVE_FRIEND){
//        QString id = message["userID"].toString();
//        QString name = message["userName"].toString();
//        NoticePrcsCommand* notice = new NoticePrcsCommand(type, id, name);
//        QThreadPool::globalInstance()->start(notice);
//    }
//    else if(type == RecvType::SERVER_FORWARD_PRIVATE_CHAT || type == RecvType::SERVER_FORWARD_GROUP_CHAT){
//        QString content = message["content"].toString();
//        QString timestamp = message["timestamp"].toString();
//        if(type == RecvType::SERVER_FORWARD_PRIVATE_CHAT){
//            QString id = header["senderID"].toString();
//            ForwardPrcsCommand* chat = new ForwardPrcsCommand(type, content, timestamp, id);
//            QThreadPool::globalInstance()->start(chat);
//        }
//        else{
//            QString userID = header["senderID"].toString();
//            int groupID = header["groupID"].toInt();
//            ForwardPrcsCommand* chat = new ForwardPrcsCommand(type, content, timestamp, userID, groupID);
//            QThreadPool::globalInstance()->start(chat);
//        }
//    }
//    else{
//        bool status = message["status"].toBool();
//        QJsonObject data;
//        QString reason;
//        if(message.contains("data")){
//            data = message["data"].toObject();
//        }
//        else if(message.contains("reason")){
//            reason = message["reason"].toString();
//        }
//        ReplyPrcsCommand* notice = new ReplyPrcsCommand(type, status, data, reason);
//        QThreadPool::globalInstance()->start(notice);
//    }

//        ReplyProcessCommand replyPrcs = new ReplyProcessCommand();
//        QThreadPool::globalInstance()->start(replyPrcs);


}
#endif //test
