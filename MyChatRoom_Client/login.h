#ifndef LOGIN_H
#define LOGIN_H

#include "client.h"

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

private:
    Ui::Login *ui;

    Client* m_client;
};

#endif // LOGIN_H
