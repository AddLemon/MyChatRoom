#ifndef NEWGROUP_H
#define NEWGROUP_H

#include "client.h"

#include <QDialog>

namespace Ui {
class NewGroup;
}

class NewGroup : public QDialog
{
    Q_OBJECT

public:
    explicit NewGroup(QWidget *parent = nullptr);
    ~NewGroup();

private slots:
    void on_cfmBtn_clicked();

    void on_quitBtn_clicked();

signals:
    void closed();

private:
    bool waitResponse();

private:
    Ui::NewGroup *ui;
    Client* m_client;
//    QTimer *timer;
    QEventLoop loop;
};

#endif // NEWGROUP_H
