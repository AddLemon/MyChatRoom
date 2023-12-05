#include "self.h"

Self::Self(QObject *parent)
    : QObject{parent}
{
    //init("alex123", "Alex", "123456");  //test
}

void Self::init(QString id, QString name, QString password)
{
    m_id = id;
    m_name = name;
    m_password = password;
}

QString Self::getID()
{
    return m_id;
}

QString Self::getName()
{
    return m_name;
}

QString Self::getPassword()
{
    return m_password;
}

void Self::setName(QString name)
{
    m_name = name;
}

void Self::setPassword(QString password)
{
    m_password = password;
}

