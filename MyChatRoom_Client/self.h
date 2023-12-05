#ifndef SELF_H
#define SELF_H

#include <QObject>

class Self : public QObject
{
    Q_OBJECT
public:
    explicit Self(QObject *parent = nullptr);

    void init(QString id, QString name, QString password);
    QString getID();
    QString getName();
    QString getPassword();

    void setName(QString name);
    void setPassword(QString password);
signals:

private:
    QString m_id;
    QString m_name;
    QString m_password;
};

#endif // SELF_H
