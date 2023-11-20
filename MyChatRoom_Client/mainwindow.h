#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "groupchat.h"
#include "privatechat.h"
#include "addnew.h"
#include "newgroup.h"
#include "login.h"
#include "recvprocessor.h"
#include "recvmanager.h"

#include <QMainWindow>
#include <QString>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void restart();
    void showLogIn();
public slots:
    void onFdListClicked(const QModelIndex &index);
    void onGpListClicked(const QModelIndex &index);

private slots:
    void on_actionadd_friend_group_triggered();

    void on_actioncreate_group_triggered();

    void on_action_Logout_triggered();

private:
    Ui::MainWindow *ui;

    //ui
    Login* ui_login;
    AddNew* ui_add;
    NewGroup* ui_new;
    QMap<QString, PrivateChat*> m_activePrivateChats;
    QMap<int, GroupChat*> m_activeGroupChats;

    //data
    Client* m_client;
    Self* m_self;
    FriendsModel* m_friendsModel;
    GroupsModel* m_groupsModel;

    //recv
    RecvProcessor* m_recvProcessor;
    RecvManager* m_recvManager;
    QThread* m_recvPcsThread;
    QThread* m_recvMngThread;
    Socket* m_socket;
};
#endif // MAINWINDOW_H
