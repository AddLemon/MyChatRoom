#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include "public.h"

class FriendsModel : public QObject
{
    Q_OBJECT
public:
    explicit FriendsModel(QObject *parent = nullptr);

    QStandardItemModel* getModel();

    void init();
    void load(QList<User> friends);
    void add(User user);
    void remove(QString id);
    void renewStatus(QString id, bool status);
    void changeWindowStatus(QString id, bool status);
    bool getWindowStatus(QString id);
    void addMsgNumber(QString id);
    void clearMsgNumber(QString id);
    QStandardItem* searchItem(QString id);
    QString searchName(QString id);

signals:

private:
    QStandardItemModel *m_model;

};

#endif // FRIENDSMODEL_H
