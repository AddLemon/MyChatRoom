#include "listdelegate.h"
#include "sendcommands.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QMessageBox>
#include <QThreadPool>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init
    ui_add = nullptr;
    ui_new = nullptr;

    m_client = Client::getInstance();
    m_self = m_client->m_self;
    m_friendsModel = m_client->m_friendsModel;
    m_groupsModel = m_client->m_groupsModel;
    //m_friendsModel->init(); //test
    //m_groupsModel->init();  //test

    //bind model and delegate for the view
    ui->fdListView->setModel(m_friendsModel->getModel());
    ui->gpListView->setModel(m_groupsModel->getModel());

    ui->fdListView->setItemDelegate(new listDelegate(this));
    ui->gpListView->setItemDelegate(new listDelegate(this));

    //clicked list
    connect(ui->fdListView, &QListView::clicked, this, &MainWindow::onFdListClicked);
    connect(ui->gpListView, &QListView::clicked, this, &MainWindow::onGpListClicked);

    //right clicked list
    ui->fdListView->setContextMenuPolicy(Qt::CustomContextMenu);   //test
    connect(ui->fdListView, &QListView::customContextMenuRequested, this, [=](const QPoint &pos){
        QModelIndex index = ui->fdListView->indexAt(pos);

        // 创建菜单, 菜单项是 QAction 类型
        QMenu menu;
        QAction* act = menu.addAction(QIcon(":/res/people-minus-one.png"), "Delete");
        connect(act, &QAction::triggered, this, [=]()
                {
                    QString name = index.data(CustomRoles::nameRole).toString();
                    int ret = QMessageBox::question(this, "Delete",
                                                    "Are you sure to delete "+name+" from your list?",
                                                    QMessageBox::Yes|QMessageBox::Cancel,
                                                    QMessageBox::Cancel);
                    //确认删除
                    if(ret == QMessageBox::Yes){
                        QString id = index.data(CustomRoles::idRole).toString();

                        //创建任务对象并加入线程池
                        rmvFrndCommand* cmd = new rmvFrndCommand(id);
                        QThreadPool* tpool = QThreadPool::globalInstance();
                        tpool->start(cmd);
                    }
                });

        // 模态显示右键菜单
        menu.exec(ui->fdListView->viewport()->mapToGlobal(pos));
    });

    //登录成功
    connect(m_client, &Client::loginSuccessful, this, [=](){
        this->show();
    });

    //连接服务器
    m_socket = new Socket(this);
    connect(m_client, &Client::readySend, m_socket, &Socket::write);
    connect(m_socket, &Socket::disconnected, this, [=](){
        QMessageBox::critical(this, "Disconnected", "Server disconnected..");
    });
    connect(m_socket, &Socket::failInitEncry, this, [=](){
        QMessageBox::warning(ui_login, "Failed", "Failed to init encryptor, please restart..");
    });

    //接收服务器消息
    m_recvPcsThread = new QThread(this);
    m_recvMngThread = new QThread(this);

    m_recvProcessor = new RecvProcessor();
    m_recvManager = new RecvManager();

    m_recvProcessor->moveToThread(m_recvPcsThread);
    m_recvManager->moveToThread(m_recvMngThread);

    connect(m_socket, &Socket::received, m_recvProcessor, &RecvProcessor::process);

    connect(m_client, &Client::newPrivateMsg, m_recvManager, [=](QString friendID, Message msg){
        m_recvManager->processNewMsg(friendID, msg);
    });

    connect(m_client, &Client::newGroupMsg, m_recvManager, [=](int groupID, Message msg){
        m_recvManager->processNewMsg(groupID, msg);
    });

    m_recvPcsThread->start();
    m_recvMngThread->start();

    //显示登录界面
    ui_login = new Login();
    ui_login->show();
}

MainWindow::~MainWindow()
{
//    //send log off signal
//    logOffCommand* cmd = new logOffCommand();
//    QThreadPool* tpool = QThreadPool::globalInstance();
//    tpool->start(cmd);
//    tpool->waitForDone(100);

    //ui
    ui_login->deleteLater();
    ui_add->deleteLater();
    ui_new->deleteLater();
    ui_add = nullptr;
    ui_new = nullptr;
    qDeleteAll(m_activePrivateChats);
    qDeleteAll(m_activeGroupChats);
    m_activePrivateChats.clear();
    m_activeGroupChats.clear();
    //data
    m_self = nullptr;
    m_friendsModel = nullptr;
    m_groupsModel = nullptr;
    //view deconnect model
    ui->fdListView->setModel(nullptr);
    ui->gpListView->setModel(nullptr);
    delete ui;
}

void MainWindow::showLogIn()
{
//    //显示登录界面
//    ui_login = new Login(this);
//    if (ui_login->exec() == QDialog::Rejected) {
//        close(); // 关闭主界面并结束程序
//    }
//    else {
//        show(); // 登录成功，显示主界面
//    }
}

void MainWindow::restart()
{
    this->hide();
    //ui
    ui_add->deleteLater();
    ui_new->deleteLater();
    ui_add = nullptr;
    ui_new = nullptr;
    qDeleteAll(m_activePrivateChats);
    qDeleteAll(m_activeGroupChats);
    m_activePrivateChats.clear();
    m_activeGroupChats.clear();
    //data
    m_self = nullptr;
    m_friendsModel = nullptr;
    m_groupsModel = nullptr;
    //view deconnect model
    ui->fdListView->setModel(nullptr);
    ui->gpListView->setModel(nullptr);

    //restart client
    m_client->restart();
    //init data again
    m_self = m_client->m_self;
    m_friendsModel = m_client->m_friendsModel;
    m_groupsModel = m_client->m_groupsModel;
    //bind model and delegate for the view
    ui->fdListView->setModel(m_friendsModel->getModel());
    ui->gpListView->setModel(m_groupsModel->getModel());

    //show login dialog
    ui_login->show();
}

void MainWindow::onFdListClicked(const QModelIndex &index)
{
    QString id = index.data(CustomRoles::idRole).toString();
    if(m_activePrivateChats.contains(id)){
        m_activePrivateChats[id]->show();
        m_friendsModel->changeWindowStatus(id, true);   //改变model中的窗口状态为true
        m_friendsModel->clearMsgNumber(id);         //清空model中的未读消息数
    }
    else{
        PrivateChat* ui_pvChat = new PrivateChat(index.data(idRole).toString(), this);
        connect(m_recvManager, &RecvManager::newPrivateMsg, ui_pvChat, &PrivateChat::showNewMsg);   //recv new msg
        connect(ui_pvChat, &PrivateChat::rejected, this, [=](){
            m_friendsModel->changeWindowStatus(id, false);   //改变model中的窗口状态为false
        });

        ui_pvChat->setObjectName(id);               //bind id with chatWindow
        m_recvManager->newActiveWindow(id);         //recvManager record
        m_activePrivateChats.insert(id, ui_pvChat); //mainwindow record
        m_friendsModel->changeWindowStatus(id, true);   //改变model中的窗口状态为true
        m_friendsModel->clearMsgNumber(id);         //清空model中的未读消息数
        ui_pvChat->show();
    }
}

void MainWindow::onGpListClicked(const QModelIndex &index)
{
    int id = index.data(CustomRoles::idRole).toInt();
    if(m_activeGroupChats.contains(id)){
        m_activeGroupChats[id]->show();
        m_groupsModel->changeWindowStatus(id, true);   //改变model中的窗口状态为true
        m_groupsModel->clearMsgNumber(id);         //清空model中的未读消息数
    }
    else{
        GroupChat* ui_gpChat = new GroupChat(index.data(idRole).toInt(), this);
        connect(m_recvManager, &RecvManager::newGroupMsg, ui_gpChat, &GroupChat::showNewMsg);   //recv new msg
        connect(ui_gpChat, &GroupChat::rejected, this, [=](){
            m_groupsModel->changeWindowStatus(id, false);   //改变model中的窗口状态为false
        });

        ui_gpChat->setObjectName(QString::number(id));  //bind id with chatWindow
        m_recvManager->newActiveWindow(id);             //recvManager record
        m_activeGroupChats.insert(id, ui_gpChat);       //mainwindow record
        m_groupsModel->changeWindowStatus(id, true);   //改变model中的窗口状态为true
        m_groupsModel->clearMsgNumber(id);         //清空model中的未读消息数
        ui_gpChat->show();
    }
}

void MainWindow::on_actionadd_friend_group_triggered()
{
    if(ui_add != nullptr){
        ui_add->show();
    }
    else{
        ui_add = new AddNew(this);
        //close the window and delete
        connect(ui_add, &AddNew::rejected, this, [&](){
            ui_add->deleteLater();  //点击quit按钮会删除窗口
            ui_add = nullptr;
        });
        ui_add->show();
    }
}




void MainWindow::on_actioncreate_group_triggered()
{
    if(ui_new != nullptr){
        ui_new->show();
    }
    else{
        ui_new = new NewGroup(this);
        //close the window and delete
        connect(ui_new, &NewGroup::closed, this, [&](){
            ui_new->deleteLater();  //点击quit按钮会删除窗口
            ui_new = nullptr;
        });
        ui_new->show();
    }
}


void MainWindow::on_action_Logout_triggered()
{
    logOffCommand* cmd = new logOffCommand();
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);
    restart();
}

