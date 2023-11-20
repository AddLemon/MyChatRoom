#include "addnew.h"
#include "sendcommands.h"
#include "ui_addnew.h"

#include <QMessageBox>
#include <QThreadPool>
#include <QTimer>

AddNew::AddNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addNew)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); //默认显示user界面
    ui->addFBtn->setEnabled(false); //初始化添加按钮为不可点击
    ui->addGBtn->setEnabled(false); //初始化添加按钮为不可点击
    m_client = Client::getInstance();

    //get searched user info
    connect(m_client, &Client::findUserSuccessful, this, [&](QString id, QString name){
        loop.quit();

        m_userID = id;
        m_userName = name;
        ui->userTbtn->setText(m_userName);
        //ui->userTbtn->setIcon();
        ui->addFBtn->setEnabled(true); //设置添加按钮为可点击
    });

    //get searched group info
    connect(m_client, &Client::findGroupSuccessful, this, [&](int id, QString name){
        loop.quit();

        m_groupID = id;
        m_groupName = name;
        ui->groupTbtn->setText(m_groupName);
        //ui->groupTbtn->setIcon();
        ui->addGBtn->setEnabled(true); //设置添加按钮为可点击
    });

    //success to add friend
    connect(m_client, &Client::addFriendSuccessful, &loop, &QEventLoop::quit);

    //success to add friend
    connect(m_client, &Client::addGroupSuccessful, &loop, &QEventLoop::quit);

    //failed to search user
    connect(m_client, &Client::findUserFailed, this, [=](QString reason){
        loop.quit();
        QMessageBox::critical(this, "Failed", reason);
    });

    //failed to search group
    connect(m_client, &Client::findGroupFailed, this, [=](QString reason){
        loop.quit();
        QMessageBox::critical(this, "Failed", reason);
    });

    //failed to add friend
    connect(m_client, &Client::addFriendFailed, this, [=](QString reason){
        loop.quit();
        QMessageBox::critical(this, "Failed", reason);
    });

    //failed to add friend
    connect(m_client, &Client::addGroupFailed, this, [=](QString reason){
        loop.quit();
        QMessageBox::critical(this, "Failed", reason);
    });
}

AddNew::~AddNew()
{
    delete ui;
}

//search user
void AddNew::on_searchFBtn_clicked()
{
    QString id = ui->userID->text();
    findUsrCommand* cmd = new findUsrCommand(id);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    if(!waitResponse()){
        QMessageBox::critical(this, "Failed", "Server not responding");
    }
}

//add user
void AddNew::on_addFBtn_clicked()
{
    addFrndCommand* cmd = new addFrndCommand(m_userID);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    if(!waitResponse()){
        QMessageBox::critical(this, "Failed", "Server not responding");
    }
}

//search group
void AddNew::on_searchGBtn_clicked()
{
    int id = ui->groupID->text().toInt();
    findGrpCommand* cmd = new findGrpCommand(id);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    if(!waitResponse()){
        QMessageBox::critical(this, "Failed", "Server not responding");
    }
}

//add group
void AddNew::on_addGBtn_clicked()
{
    addGrpCommand* cmd = new addGrpCommand(m_groupID);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    if(!waitResponse()){
        QMessageBox::critical(this, "Failed", "Server not responding");
    }
}

bool AddNew::waitResponse()
{
    // 初始化定时器
    QTimer timer;
    timer.setSingleShot(true); // 单次触发

    setEnabled(false);      //阻塞窗口
    QApplication::setOverrideCursor(Qt::WaitCursor);    //指针变为加载状态
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(5000); // 设置超时时间为5秒

    loop.exec();

    setEnabled(true);   //停止阻塞窗口
    QApplication::restoreOverrideCursor();  //解除指针加载状态
    if (timer.isActive()) {
        // 操作完成，未超时
        timer.stop();
        return true;
    } else {
        // 操作超时
        return false;
    }
}

