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
        item->setEditable(false);
        m_model->appendRow(item);
    }
}

void FriendsModel::add(QString id, QString name)
{
    QStandardItem* item = new QStandardItem();
    item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
    item->setData(id, CustomRoles::idRole);
    item->setData(name, CustomRoles::nameRole);
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
