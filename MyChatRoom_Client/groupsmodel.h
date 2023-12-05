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
    void add(Group group);
    void remove(int id);
    void changeWindowStatus(int id, bool status);
    bool getWindowStatus(int id);
    void addMsgNumber(int id);
    void clearMsgNumber(int id);
    QStandardItem* searchItem(int id);
    QString searchName(int id);

signals:

private:
    QStandardItemModel *m_model;

};

#endif // GROUPSMODEL_H
