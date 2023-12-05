#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include "client.h"
#include "membersmodel.h"

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class GroupChat;
}

class GroupChat : public QDialog
{
    Q_OBJECT

public:
    explicit GroupChat(int groupID, QWidget *parent = nullptr);
    ~GroupChat();

    void getMembers();

    void showNewMsg(int id, Message msg);

    bool loadedMembers();

private slots:
    void on_frshBtn_clicked();  //刷新成员列表

    void on_sendBtn_clicked();

private:
    Ui::GroupChat *ui;

    Client* m_client;
    int m_groupID;
    MembersModel* m_members;  //储存成员列表的model
    QEventLoop loop;
};

#endif // GROUPCHAT_H
