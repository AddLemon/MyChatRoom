#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include "client.h"

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateChat(QString recvID, QWidget *parent = nullptr);
    ~PrivateChat();

    void showNewMsg(QString id, Message msg);

protected:


private slots:
    void on_sendBtn_clicked();

private:
    Ui::PrivateChat *ui;
    Client* m_client;
    QString m_senderID;
    QString m_receiverID;
    QString m_receiverName;
};

#endif // PRIVATECHAT_H
