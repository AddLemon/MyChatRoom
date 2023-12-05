#include "friendsmodel.h"

#include <QIcon>
#include <QPixmap>
#include <QApplication>

FriendsModel::FriendsModel(QObject *parent)
    : QObject{parent}
{
    m_model = new QStandardItemModel(this);
}

QStandardItemModel *FriendsModel::getModel()
{
    return m_model;
}

void FriendsModel::init()
{
    for(int i=0; i<10; i++){
        QStandardItem* item = new QStandardItem();
        item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
        item->setData(QString("id%1").arg(i), CustomRoles::idRole);
        item->setData(QString("name%1").arg(i), CustomRoles::nameRole);
        item->setEditable(false);
        m_model->appendRow(item);
    }
}

void FriendsModel::load(QList<User> friends)
{
    m_model->clear();
    for(int i=0; i<friends.size(); i++){
        User m = friends.first();
        friends.pop_front();
        QStandardItem* item = new QStandardItem();
        if(m.avatar.isNull()){
            item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
        }
        else{
            item->setData(QIcon(m.avatar), Qt::DecorationRole);
        }
        item->setData(m.id, CustomRoles::idRole);
        item->setData(m.name, CustomRoles::nameRole);
        item->setData(m.status, CustomRoles::statusRole);
        item->setData(false, CustomRoles::winStatusRole);
        item->setData(0, CustomRoles::msgNumberRole);
        item->setEditable(false);
        m_model->appendRow(item);
    }
}

void FriendsModel::add(User user)
{
    QStandardItem* item = new QStandardItem();
    item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
    item->setData(user.id, CustomRoles::idRole);
    item->setData(user.name, CustomRoles::nameRole);
    item->setData(user.status, CustomRoles::statusRole);
    item->setData(false, CustomRoles::winStatusRole);
    item->setData(0, CustomRoles::msgNumberRole);
    item->setEditable(false);
    m_model->appendRow(item);
}

void FriendsModel::remove(QString id)
{
    QStandardItem* item = searchItem(id);
    if(item == nullptr){
        return; //or messageBox
    }
    m_model->removeRow(item->row());
}

void FriendsModel::renewStatus(QString id, bool status)
{
    QStandardItem* item = searchItem(id);
    item->setData(QVariant(status), CustomRoles::statusRole);
    //emit m_model->dataChanged(item->index(), item->index(), {CustomRoles::statusRole});
}

void FriendsModel::changeWindowStatus(QString id, bool status)
{
    QStandardItem* item = searchItem(id);
    item->setData(QVariant(status), CustomRoles::winStatusRole);
}

bool FriendsModel::getWindowStatus(QString id)
{
    QStandardItem* item = searchItem(id);
    return item->data(CustomRoles::winStatusRole).toBool();
}

void FriendsModel::addMsgNumber(QString id)
{
    QStandardItem* item = searchItem(id);
    int i = item->data(CustomRoles::msgNumberRole).toInt();
    item->setData(QVariant(++i), CustomRoles::msgNumberRole);
}

void FriendsModel::clearMsgNumber(QString id)
{
    QStandardItem* item = searchItem(id);
    item->setData(QVariant(0), CustomRoles::msgNumberRole);
}

QStandardItem *FriendsModel::searchItem(QString id)
{
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem* item = m_model->item(row);
        if (item && item->data(CustomRoles::idRole) == id) {
            return item;
        }
    }
    return nullptr;
}

QString FriendsModel::searchName(QString id)
{
    return searchItem(id)->data(CustomRoles::nameRole).toString();
}
