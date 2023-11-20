#include "privatechat.h"
#include "sendcommands.h"
#include "ui_privatechat.h"

#include <QDateTime>
#include <QThreadPool>

PrivateChat::PrivateChat(QString recvID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    m_client = Client::getInstance();
    m_senderID = m_client->m_self->getID();
    m_receiverID = recvID;
    m_receiverName = m_client->m_friendsModel->searchItem(recvID)->data(nameRole).toString();
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::showNewMsg(QString id, Message msg)
{
    if(id == this->objectName()){
        //时间戳转换
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(msg.timestamp.toLongLong());
        QString dateTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");

        //qDebug() << "Formatted DateTime:" << formattedDateTime;
        ui->msgBrowser->append("["+m_receiverName+"]"+dateTimeStr);
        ui->msgBrowser->append(msg.content);
    }
}

void PrivateChat::on_sendBtn_clicked()
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
    QString timestampStr = QString::number(timestamp);

    pvChatCommand* cmd = new pvChatCommand(m_receiverID, content, timestampStr);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

//    //test
//    Message a;
//    a.userID = m_senderID;
//    a.content = content;
//    a.timestamp = timestampStr;
//    showNewMsg(a);
}



