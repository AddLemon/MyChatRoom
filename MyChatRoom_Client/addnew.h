#ifndef ADDNEW_H
#define ADDNEW_H

#include "client.h"

#include <QDialog>

namespace Ui {
class addNew;
}

class AddNew : public QDialog
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = nullptr);
    ~AddNew();

private slots:
    void on_searchFBtn_clicked();

    void on_addFBtn_clicked();

    void on_searchGBtn_clicked();

    void on_addGBtn_clicked();

private:
    bool waitResponse();

private:
    Ui::addNew *ui;
    Client* m_client;
    QEventLoop loop;

    QString m_userID;
    QString m_userName;
    int m_groupID;
    QString m_groupName;
};

#endif // ADDNEW_H
