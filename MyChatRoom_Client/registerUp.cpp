#include "registerUp.h"
#include "sendcommands.h"
#include "ui_registerUp.h"

#include <QThreadPool>

RegisterUp::RegisterUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterUp)
{
    ui->setupUi(this);

    m_client = Client::getInstance();

    //连接client信号
}

RegisterUp::~RegisterUp()
{
    delete ui;
}

void RegisterUp::on_cfmBtn_clicked()
{
    QString id = ui->id->text();
    QString name = ui->name->text();
    QString pswd_a = ui->pswd->text();
    QString pswd_b = ui->pswd_again->text();

    //判断是否都输入了数据
    if(id.isEmpty() || id.isEmpty() || id.isEmpty() || id.isEmpty()){
        ui->checkLabel->setText("<font color='red'>Make sure to fill in every box..</font>");
        return;
    }

    //判断两次密码是否相同
    if(pswd_a != pswd_b){
        ui->checkLabel->setText("<font color='red'>The passwords filled in twice are not the same..</font>");
        ui->pswd->setText("");
        ui->pswd_again->setText("");
        return;
    }

    //创建任务类并加入线程池
    RegisterCommand* cmd = new RegisterCommand(id, name, pswd_a);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    //阻塞并等待反馈信号
    QEventLoop loop;
    QObject::connect(m_client, &Client::registerSuccessful, &loop, [&](){
        loop.quit();
        ui->id->setText("");
        ui->name->setText("");
        ui->pswd->setText("");
        ui->pswd_again->setText("");
        ui->checkLabel->setText("<font color='green'>Registration successful! please close this window..</font>");
    });
    QObject::connect(m_client, &Client::registerFailed, &loop, [&](QString reason){
        loop.quit();
        ui->checkLabel->setText("<font color='red'>"+reason+"</font>");
    });
    loop.exec();
}

