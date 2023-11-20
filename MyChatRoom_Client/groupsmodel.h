#ifndef GROUPSMODEL_H
#define GROUPSMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include "public.h"

class GroupsModel : public QObject
{
    Q_OBJECT
public:
    explicit GroupsModel(QObject *parent = nullptr);

    QStandardItemModel* getModel();

    void init();
    void load(QList<Group> groups);
    void add(int id, QString name);
    void remove(int id);
    QStandardItem* searchItem(int id);
    QString searchName(int id);

signals:

private:
    QStandardItemModel *m_model;

};

#endif // GROUPSMODEL_H
