#include "membersmodel.h"


MembersModel::MembersModel(QObject *parent)
    : QObject{parent}
{
    m_model = new QStandardItemModel(this);
}

QStandardItemModel *MembersModel::getModel()
{
    return m_model;
}

void MembersModel::init()
{
    for(int i=0; i<5; i++){
        QStandardItem* item = new QStandardItem();
        item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
        item->setData(QString("id%1").arg(i), CustomRoles::idRole);
        item->setData(QString("name%1").arg(i), CustomRoles::nameRole);
        item->setEditable(false);
        m_model->appendRow(item);
    }
}

void MembersModel::load(QList<User> members)
{
    m_model->clear();
    //for(User i : members){
    for(int a=0; a<=members.size(); a++){
        User i = members.first();
        members.pop_front();

        QStandardItem* item = new QStandardItem();
        if(i.avatar.isNull()){
            item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
        }
        else{
            item->setData(QIcon(i.avatar), Qt::DecorationRole);
        }
        item->setData(i.id, CustomRoles::idRole);
        item->setData(i.name, CustomRoles::nameRole);
        item->setEditable(false);
        m_model->appendRow(item);
        qDebug() << "group member: " << item->data(CustomRoles::nameRole).toString();
    }
}

void MembersModel::add(QString id, QString name)
{
    QStandardItem* item = new QStandardItem();
    item->setData(QIcon(":/res/avatar.png"), Qt::DecorationRole);
    item->setData(id, CustomRoles::idRole);
    item->setData(name, CustomRoles::nameRole);
    item->setEditable(false);
    m_model->appendRow(item);
}

void MembersModel::remove(QString id)
{
    QStandardItem* item = searchItem(id);
    if(item == nullptr){
        return; //or messageBox
    }
    m_model->removeRow(item->row());
}

QStandardItem *MembersModel::searchItem(QString id)
{
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem* item = m_model->item(row);
        if (item && item->data(CustomRoles::idRole) == id) {
            return item;
        }
    }
    return nullptr;
}

QString MembersModel::searchName(QString id)
{
    return searchItem(id)->data(CustomRoles::nameRole).toString();
}
