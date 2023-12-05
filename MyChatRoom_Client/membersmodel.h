#ifndef MEMBERSMODEL_H
#define MEMBERSMODEL_H

#include <QQueue>
#include <QObject>
#include <QStandardItemModel>
#include "public.h"


class MembersModel : public QObject
{
    Q_OBJECT
public:
    explicit MembersModel(QObject *parent = nullptr);

    QStandardItemModel* getModel();

    void init();    //test
    void load(QList<User> members);
    void add(QString id, QString name);
    void remove(QString id);
    QStandardItem* searchItem(QString id);
    QString searchName(QString id);

signals:

private:
    QStandardItemModel *m_model;

};

#endif // MEMBERSMODEL_H
