#include "newgroup.h"
#include "sendcommands.h"
#include "ui_newgroup.h"

#include <QMessageBox>
#include <QThreadPool>
#include <QPixmap>
#include <QTimer>

NewGroup::NewGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGroup)
{
    ui->setupUi(this);
    ui->cfmBtn->setEnabled(false);  //确认按钮默认不可点击

    m_client = Client::getInstance();

    //enable confirm button
    connect(ui->nameEdit, &QLineEdit::textEdited, this, [=](const QString &text){
        if(!text.isEmpty()){
            ui->cfmBtn->setEnabled(true); //信息输入后按钮变为可点击
        }
        else{
            ui->cfmBtn->setEnabled(false);  //将确认按钮设置为不可点击
        }
    });

    //success to create
    connect(m_client, &Client::crtGroupSuccessful, this, [=](int id, QString name){
        loop.quit();

        ui->nameEdit->clear();  //清空已输入的文本

        QPixmap pix = QPixmap(":/res/correct.png").scaled(ui->rstLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->iconLabel->setPixmap(pix);
        ui->rstLabel->setText("<font color='green'>Success! Group ID: "+QString::number(id)+"</font>");

        //QMessageBox::information(this, "Information", "Success to create a group: "+name+", group id is"+QString::number(id));
    });

    //failed to create
    connect(m_client, &Client::crtGroupFailed, this, [=](QString reason){
        loop.quit();

        QPixmap pix = QPixmap(":/res/error.png").scaled(ui->rstLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->iconLabel->setPixmap(pix);
        ui->rstLabel->setText("<font color='red'>"+reason+"</font>");
    });
}

NewGroup::~NewGroup()
{
    delete ui;
}

void NewGroup::on_cfmBtn_clicked()
{
    QString name = ui->nameEdit->text();
    crtGrpCommand* cmd = new crtGrpCommand(name);
    QThreadPool* tpool = QThreadPool::globalInstance();
    tpool->start(cmd);

    if(!waitResponse()){
        // 操作超时
        QPixmap pix = QPixmap(":/res/error.png").scaled(ui->rstLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->iconLabel->setPixmap(pix);
        ui->rstLabel->setText("<font color='red'>no response</font>");
    }
}


void NewGroup::on_quitBtn_clicked()
{
    QApplication::restoreOverrideCursor();  //解除指针加载状态
    emit closed();
}

bool NewGroup::waitResponse()
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


