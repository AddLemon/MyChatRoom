#include "groupchat.h"
#include "sendcommands.h"
#include "membersdelegate.h"
#include "ui_groupchat.h"

#include <QThreadPool>
#include <QDebug>
#include <QPixmap>

GroupChat::GroupChat(int groupID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupChat)
{
    ui->setupUi(this);

    m_client = Client::getInstance();
    m_groupID = groupID;
    m_members = nullptr;

    connect(m_client, &Client::getMembersSuccessful, this, [=](){
        loop.quit();
        qDebug() << "start set model"; //test
        //bind model and delegate for the view
        m_members = m_client->m_membersModelMap.value(groupID);
        ui->mbrLView->setModel(m_members->getModel());
        ui->mbrLView->setItemDelegate(new MembersDelegate(this));
        qDebug() << "finish set model"; //test
    });

    connect(m_client, &Client::frshMembersSuccessful, this, [=](){
        ui->frshLabel->setPixmap(QPixmap(":/res/correct.png").scaled(ui->frshLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    connect(m_client, &Client::getMembersFailed, this, [=](){
        loop.quit();
        ui->frshLabel->setPixmap(QPixmap(":/res/caution.png").scaled(ui->frshLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    getMembers();   //通过客户端向服务器请求获取成员列表

    loop.exec();    //等待列表加载完成
}

GroupChat::~GroupChat()
{
    delete ui;
}

void GroupChat::getMembers()
{
    getGrpMbrCommand* cmd = new getGrpMbrCommand(m_groupID);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);
}

void GroupChat::showNewMsg(int id, Message msg)
{
    if(QString::number(id) == this->objectName()){
        QString senderName = m_members->searchName(msg.userID);
        //时间戳转换
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(msg.timestamp.toLongLong());
        QString dateTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");

        //qDebug() << "Formatted DateTime:" << formattedDateTime;
        ui->msgBrowser->append("["+senderName+"]"+dateTimeStr);
        ui->msgBrowser->append(msg.content);
    }
}

bool GroupChat::loadedMembers()
{
    if(m_members == nullptr){
        return false;
    }
    return true;
}

void GroupChat::on_frshBtn_clicked()
{
    getMembers();
}

void GroupChat::on_sendBtn_clicked()
{
    if(ui->msgTxtEdit->document()->isEmpty()){
        return; //内容为空，不进行操作
    }
    QString content = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();

    QDateTime currentTime = QDateTime::currentDateTime();
    // 获取时间戳
    qint64 timestamp = currentTime.toSecsSinceEpoch();
    QString dateTimeStr = QDateTime::fromSecsSinceEpoch(timestamp).toString("yyyy-MM-dd HH:mm:ss");
    QString timestampStr = QString::number(timestamp);

    gpChatCommand* cmd = new gpChatCommand(m_groupID, content, timestampStr);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    ui->msgBrowser->append("[Me]"+dateTimeStr);
    ui->msgBrowser->append(content);
//        //test
//        Message a;
//        a.userID = "id1";
//        a.content = content;
//        a.timestamp = timestampStr;
//        showNewMsg(a);
}




