#include "groupsmodel.h"

GroupsModel::GroupsModel(QObject *parent)
    : QObject{parent}
{
    m_model = new QStandardItemModel(this);
}

QStandardItemModel *GroupsModel::getModel()
{
    return m_model;
}

void GroupsModel::init()
{
    for(int i=0; i<3; i++){
        QStandardItem* item = new QStandardItem();
        item->setData(QIcon(":/res/peoples-two.png"), Qt::DecorationRole);
        item->setData(QString("%1").arg(i*100000), CustomRoles::idRole);
        item->setData(QString("group %1").arg(i), CustomRoles::nameRole);
        item->setEditable(false);
        m_model->appendRow(item);
    }
}

void GroupsModel::load(QList<Group> groups)
{
    m_model->clear();
    for(int i=0; i<groups.size(); i++){
        Group m = groups.first();
        groups.pop_front();
        QStandardItem* item = new QStandardItem();
        if(m.avatar.isNull()){
            item->setData(QIcon(":/res/peoples-two.png"), Qt::DecorationRole);
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

void GroupsModel::add(Group group)
{
    QStandardItem* item = new QStandardItem();
    item->setData(QIcon(":/res/peoples-two.png"), Qt::DecorationRole);
    item->setData(group.id, CustomRoles::idRole);
    item->setData(group.name, CustomRoles::nameRole);
    item->setEditable(false);
    m_model->appendRow(item);
}

void GroupsModel::remove(int id)
{
    QStandardItem* item = searchItem(id);
    if(item == nullptr){
        return; //or messageBox
    }
    m_model->removeRow(item->row());
}

void GroupsModel::changeWindowStatus(int id, bool status)
{
    QStandardItem* item = searchItem(id);
    item->setData(QVariant(status), CustomRoles::winStatusRole);
}

bool GroupsModel::getWindowStatus(int id)
{
    QStandardItem* item = searchItem(id);
    return item->data(CustomRoles::winStatusRole).toBool();
}

void GroupsModel::addMsgNumber(int id)
{
    QStandardItem* item = searchItem(id);
    int i = item->data(CustomRoles::msgNumberRole).toInt();
    item->setData(QVariant(++i), CustomRoles::msgNumberRole);
}

void GroupsModel::clearMsgNumber(int id)
{
    QStandardItem* item = searchItem(id);
    item->setData(QVariant(0), CustomRoles::msgNumberRole);
}

QStandardItem *GroupsModel::searchItem(int id)
{
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem* item = m_model->item(row);
        if (item && item->data(CustomRoles::idRole) == id) {
            return item;
        }
    }
    return nullptr;
}

QString GroupsModel::searchName(int id)
{
    return searchItem(id)->data(CustomRoles::nameRole).toString();
}
