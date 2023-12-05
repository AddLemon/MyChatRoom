#include "login.h"
#include "registerUp.h"
#include "sendcommands.h"
#include "ui_login.h"

#include <QThreadPool>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    m_client = Client::getInstance();

    //登录成功
    connect(m_client, &Client::loginSuccessful, this, [=](){
        this->hide();
        ui->pswdLEdit->clear();
    });
}

Login::~Login()
{
    delete ui;
}

void Login::on_loginBtn_clicked()
{
    QString id = ui->idLEdit->text();
    QString password = ui->pswdLEdit->text();

    //TODO check not empty

    logInCommand* cmd = new logInCommand(id, password);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    //阻塞并等待反馈信号
    QEventLoop loop;
    QObject::connect(m_client, &Client::loginSuccessful, &loop, [&](){
        loop.quit();
        ui->checkLabel->setText("");
        //ui->checkLabel->setText("<font color='green'>Registration successful! please close this window..</font>");
    });
    QObject::connect(m_client, &Client::loginFailed, &loop, [&](QString reason){
        loop.quit();
        ui->checkLabel->setText("<font color='red'>"+reason+"</font>");
    });
    loop.exec();
}


void Login::on_registerBtn_clicked()
{
    RegisterUp* ui_register = new RegisterUp(this);
    connect(ui_register, &RegisterUp::rejected, this, [=](){
        this->show();
    });
    this->hide();
    ui_register->show();
}

