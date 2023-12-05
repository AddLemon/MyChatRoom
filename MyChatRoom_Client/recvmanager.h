#ifndef RECVMANAGER_H
#define RECVMANAGER_H

#include "public.h"
#include <QMap>
#include <QObject>
#include <QRunnable>
#include <QString>

#if 1   //test

class RecvManager : public QObject
{
    Q_OBJECT
public:
    explicit RecvManager(QObject *parent = nullptr)
        : QObject(parent){};

    //检查是否已存在窗口
    bool isActiveWindow(QString friendID);
    bool isActiveWindow(int groupID);
    //检查是否有暂存消息
    bool hasUnreadMsg(QString friendID);
    bool hasUnreadMsg(int groupID);

    //储存暂存数据
    void storeMsg(QString friendID, Message msg);
    void storeMsg(int groupID, Message msg);
    //提取暂存数据
    Message takeMsg(QString friendID);
    Message takeMsg(int groupID);

    //获得暂存消息的数量
    int getMessageNumber(QString friendID); //unused
    int getMessageNumber(int groupID);      //unused

    //记录新增的窗口
    void newActiveWindow(QString friendID);
    void newActiveWindow(int groupID);
    //删除关闭的窗口
    void deleteDeactiveWindow(QString friendID);
    void deleteDeactiveWindow(int groupID);

    //处理收到的信息
    void processNewMsg(QString friendID, Message msg);
    void processNewMsg(int groupID, Message msg);
signals:
    void newPrivateMsg(QString friendID, Message msg);
    void newGroupMsg(int groupID, Message msg);
private:
    QMap<QString, QList<Message>> m_pvMessages;
    QMap<int, QList<Message>> m_gpMessages;
    QList<QString> m_activePvWindows;
    QList<int> m_activeGpWindows;
};
#endif  //test
#endif // RECVMANAGER_H
