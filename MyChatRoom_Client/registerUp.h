#ifndef REGISTERUP_H
#define REGISTERUP_H

#include "client.h"

#include <QDialog>

namespace Ui {
class RegisterUp;
}

class RegisterUp : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterUp(QWidget *parent = nullptr);
    ~RegisterUp();

private slots:
    void on_cfmBtn_clicked();

private:
    Ui::RegisterUp *ui;
    Client* m_client;
};

#endif // REGISTERUP_H
