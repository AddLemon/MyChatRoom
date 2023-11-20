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
    void add(QString id, QString name);
    void remove(QString id);
    QStandardItem* searchItem(QString id);
    QString searchName(QString id);

signals:

private:
    QStandardItemModel *m_model;

};

#endif // FRIENDSMODEL_H
