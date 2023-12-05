#include "recvmanager.h"

#if 1   //test

bool RecvManager::isActiveWindow(QString friendID)
{
    return m_activePvWindows.contains(friendID);
}

bool RecvManager::isActiveWindow(int groupID)
{
    return m_activeGpWindows.contains(groupID);
}

bool RecvManager::hasUnreadMsg(QString friendID)
{
    return m_pvMessages.contains(friendID);
}

bool RecvManager::hasUnreadMsg(int groupID)
{
    return m_gpMessages.contains(groupID);
}

void RecvManager::storeMsg(QString friendID, Message msg)
{
    if(hasUnreadMsg(friendID)){
        //已有未读消息，则按照循序保存在msg queue的尾部
        auto i = m_pvMessages.find(friendID);
        i.value().push_back(msg);
    }
    else{
        //没有该用户的未读消息，则新建msg queue保存新的msg
        QList<Message> msgs;
        msgs.push_back(msg);
        m_pvMessages.insert(friendID, msgs);
    }
}

void RecvManager::storeMsg(int groupID, Message msg)
{
    if(hasUnreadMsg(groupID)){
        //已有未读消息，则按照循序保存在msg queue的尾部
        auto i = m_gpMessages.find(groupID);
        i.value().push_back(msg);
    }
    else{
        //没有该群聊的未读消息，则新建msg queue保存新的msg
        QList<Message> msgs;
        msgs.push_back(msg);
        m_gpMessages.insert(groupID, msgs);
    }
}

Message RecvManager::takeMsg(QString friendID)
{
    //从map提取一条msg
    auto i = m_pvMessages.find(friendID);
    Message msg = i.value().front();
    i.value().pop_front();

    //若所有数据都被提取，则从map消除
    if(i.value().size() == 0){
        m_pvMessages.take(friendID);
    }
    return msg;
}

Message RecvManager::takeMsg(int groupID)
{
    //从map提取一条msg
    auto i = m_gpMessages.find(groupID);
    Message msg = i.value().front();
    i.value().pop_front();

    //若所有数据都被提取，则从map消除
    if(i.value().size() == 0){
        m_gpMessages.take(groupID);
    }
    return msg;
}

int RecvManager::getMessageNumber(QString friendID)
{
    auto i = m_pvMessages.find(friendID);
    int num = i.value().size();
    return num;
}

int RecvManager::getMessageNumber(int groupID)
{
    auto i = m_gpMessages.find(groupID);
    int num = i.value().size();
    return num;
}

void RecvManager::newActiveWindow(QString friendID)
{
    m_activePvWindows.append(friendID);
    //将所有未读消息依次发送给窗口
    while(hasUnreadMsg(friendID)){
        Message msg = takeMsg(friendID);
        emit newPrivateMsg(friendID, msg);
    }
}

void RecvManager::newActiveWindow(int groupID)
{
    m_activeGpWindows.append(groupID);
    //将所有未读消息依次发送给窗口
    while(hasUnreadMsg(groupID)){
        Message msg = takeMsg(groupID);
        emit newGroupMsg(groupID, msg);
    }
}

void RecvManager::deleteDeactiveWindow(QString friendID)
{
    //删除所有未读消息
    while(hasUnreadMsg(friendID)){
        //将所有未读消息一次发送给窗口
        takeMsg(friendID);
    }
    m_activePvWindows.removeOne(friendID);
}

void RecvManager::deleteDeactiveWindow(int groupID)
{
    //删除所有未读消息
    while(hasUnreadMsg(groupID)){
        //将所有未读消息一次发送给窗口
        takeMsg(groupID);
    }
    m_activeGpWindows.removeOne(groupID);
}

void RecvManager::processNewMsg(QString friendID, Message msg)
{
    if(isActiveWindow(friendID)){
        //将未读消息直接发送给窗口
        emit newPrivateMsg(friendID, msg);
    }
    else{
        storeMsg(friendID, msg);
    }
}

void RecvManager::processNewMsg(int groupID, Message msg)
{
    if(isActiveWindow(groupID)){
        //将未读消息直接发送给窗口
        emit newGroupMsg(groupID, msg);
    }
    else{
        storeMsg(groupID, msg);
    }
}

#endif //test
