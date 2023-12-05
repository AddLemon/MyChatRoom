#ifndef PUBLIC_H
#define PUBLIC_H

#include <QApplication>
#include <QObject>

enum CustomRoles {
    idRole = Qt::UserRole + 1,
    nameRole = Qt::UserRole + 2,
    statusRole = Qt::UserRole + 3,
    winStatusRole = Qt::UserRole + 4,
    msgNumberRole = Qt::UserRole + 5
};

enum class RecvType {
    SERVER_REPLY_REGISTER_UP,
    SERVER_REPLY_LOG_IN,
    SERVER_REPLY_MODIFY_USER_INFO,
    SERVER_REPLY_FIND_USER,
    SERVER_REPLY_ADD_FRIEND,
    SERVER_REPLY_REMOVE_FRIEND,
    SERVER_REPLY_FIND_GROUP,
    SERVER_REPLY_ADD_GROUP,
    SERVER_REPLY_REMOVE_GROUP,
    SERVER_REPLY_GET_GROUP_MEMBERS,
    SERVER_REPLY_CREATE_GROUP,
    SERVER_REPLY_MODIFY_GROUP_INFO,
    SERVER_REPLY_PRIVATE_CHAT,
    SERVER_REPLY_GROUP_CHAT,
    SERVER_NOTICE_NEW_FRIEND,
    SERVER_NOTICE_REMOVE_FRIEND,
    SERVER_NOTICE_RENEW_STATUS,
    SERVER_FORWARD_PRIVATE_CHAT,
    SERVER_FORWARD_GROUP_CHAT,
};

enum class MsgType {
    REGISTER_UP,
    LOG_IN,
    MODIFY_USER_INFO,
    FIND_USER,
    ADD_FRIEND,
    REMOVE_FRIEND,
    FIND_GROUP,
    ADD_GROUP,
    REMOVE_GROUP,
    GET_GROUP_MEMBERS,
    CREATE_GROUP,
    MODIFY_GROUP_INFO,
    PRIVATE_CHAT,
    GROUP_CHAT,
    LOG_OFF
};

struct SelfInfo {
    QPixmap avatar;
    QString userID;
    QString userName;
    QString password;
};

struct Message{
    QString userID;
    QString content;
    QString timestamp;
};

struct User{
    QPixmap avatar;
    QString id;
    QString name;
    bool status;
};

struct Group{
    QPixmap avatar;
    int id;
    QString name;
};

class WaitCursor {
public:
    WaitCursor() {
        QApplication::setOverrideCursor(Qt::WaitCursor);
    }
    ~WaitCursor() {
        QApplication::restoreOverrideCursor();
    }
};

#endif // PUBLIC_H
