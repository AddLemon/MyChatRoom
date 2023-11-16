#include "commands.h"

Command::Command(int requestID, string senderID)
{
	m_server = Server::getInstance();
	m_requestID = requestID;
	m_senderID = senderID;
}

void Command::send()
{
    addHeader();
    addCheckSum(m_reply_pkt);
    m_server->send(m_senderID, m_reply_pkt);
}

void Command::addHeader()
{
    Json::Value header;
    header["requestID"] = m_requestID;
    header["type"] = static_cast<int>(m_reply_type);
    m_reply_pkt["header"] = header;
}

void Command::addCheckSum(Json::Value& pkt)
{
    int checkSum = 0;
    Json::FastWriter w;
    string str = w.write(pkt);
    for (char i : str) {
        checkSum += static_cast<unsigned char>(i);
    }
    pkt["checkSum"] = checkSum;
}

void RegisterCommand::execute()
{
    Json::Value msg = m_server->dealRegister(m_id, m_name, m_password);
    m_reply_pkt["message"] = msg;
}

void LogInCommand::execute()
{
    Json::Value msg = m_server->dealLogin(m_senderID, m_id, m_password);
    m_reply_pkt["message"] = msg;
}

void MdfyUsrInfoCommand::execute()
{
    Json::Value msg = m_server->dealModifyUserInfo(m_senderID, m_name, m_password);
    m_reply_pkt["message"] = msg;
}

void FindUsrCommand::execute()
{
    Json::Value msg = m_server->dealFindUser(m_id);
    m_reply_pkt["message"] = msg;
}

void AddFrndCommand::execute()
{
    Json::Value notice_msg;
    Json::Value reply_msg = m_server->dealAddFriend(m_senderID, m_id, notice_msg);
    m_reply_pkt["message"] = reply_msg;
    m_notice_pkt["message"] = notice_msg;
}

void AddFrndCommand::send()
{
    addHeader();
    addCheckSum(m_reply_pkt);
    addCheckSum(m_notice_pkt);
    m_server->send(m_senderID, m_reply_pkt);
    m_server->send(m_id, m_notice_pkt);
}

void AddFrndCommand::addHeader()
{
    Json::Value reply_header;
    reply_header["requestID"] = m_requestID;
    reply_header["type"] = static_cast<int>(m_reply_type);
    m_reply_pkt["header"] = reply_header;

    Json::Value notice_header;
    notice_header["type"] = static_cast<int>(m_notice_type);
    m_notice_pkt["header"] = notice_header;
}

void RmvFrndCommand::execute()
{
    Json::Value notice_msg;
    Json::Value reply_msg = m_server->dealRemoveFriend(m_senderID, m_id, notice_msg);
    m_reply_pkt["message"] = reply_msg;
    m_notice_pkt["message"] = notice_msg;
}

void RmvFrndCommand::send()
{
    addHeader();
    addCheckSum(m_reply_pkt);
    addCheckSum(m_notice_pkt);
    m_server->send(m_senderID, m_reply_pkt);
    m_server->send(m_id, m_notice_pkt);
}

void RmvFrndCommand::addHeader()
{
    Json::Value reply_header;
    reply_header["requestID"] = m_requestID;
    reply_header["type"] = static_cast<int>(m_reply_type);
    m_reply_pkt["header"] = reply_header;

    Json::Value notice_header;
    notice_header["type"] = static_cast<int>(m_notice_type);
    m_notice_pkt["header"] = notice_header;
}

void FindGrpCommand::execute()
{
    Json::Value msg = m_server->dealFindGroup(m_id);
    m_reply_pkt["message"] = msg;
}

void RmvGrpCommand::execute()
{
    Json::Value msg = m_server->dealRemoveGroup(m_senderID, m_id);
    m_reply_pkt["message"] = msg;
}

void AddGrpCommand::execute()
{
    Json::Value msg = m_server->dealAddGroup(m_senderID, m_id);
    m_reply_pkt["message"] = msg;
}

void GetGrpMbrCommand::execute()
{
    Json::Value msg = m_server->dealGetMembers(m_id);
    m_reply_pkt["message"] = msg;
}

void CrtGrpCommand::execute()
{
    Json::Value msg = m_server->dealCreateGroup(m_senderID, m_groupName);
    m_reply_pkt["message"] = msg;
}

void MdfyGrpInfoCommand::execute()
{
    Json::Value msg = m_server->dealModifyGroupInfo(m_senderID, m_groupID, m_groupName);
    m_reply_pkt["message"] = msg;
}

void GpChatCommand::execute()
{
    Json::Value msg = m_server->dealGroupChat(m_senderID, m_groupID, m_content, m_timestamp, m_online_members);
    m_reply_pkt["message"] = msg;
    m_forward_pkt["message"] = msg;
}

void GpChatCommand::send()
{
    addHeader();
    addCheckSum(m_reply_pkt);
    addCheckSum(m_forward_pkt);
    m_server->send(m_senderID, m_reply_pkt);
    while(m_online_members.size() != 0) {
        string id = m_online_members.front();
        m_online_members.pop();
        m_server->send(id, m_forward_pkt);
    }
}

void GpChatCommand::addHeader()
{
    Json::Value reply_header;
    reply_header["requestID"] = m_requestID;
    reply_header["type"] = static_cast<int>(m_reply_type);
    m_reply_pkt["header"] = reply_header;

    Json::Value forward_header;
    forward_header["type"] = static_cast<int>(m_forward_type);
    forward_header["senderID"] = m_senderID;
    forward_header["groupID"] = m_groupID;
    m_forward_pkt["header"] = forward_header;
}

void PvChatCommand::execute()
{
    Json::Value msg = m_server->dealPrivateChat(m_senderID, m_receiverID, m_content, m_timestamp, isOL);
    m_reply_pkt["message"] = msg;
    m_forward_pkt["message"] = msg;
}

void PvChatCommand::send()
{
    addHeader();
    addCheckSum(m_reply_pkt);
    m_server->send(m_senderID, m_reply_pkt);
    if (isOL) {
        addCheckSum(m_forward_pkt);
        m_server->send(m_receiverID, m_forward_pkt);
    }
}

void PvChatCommand::addHeader()
{
    Json::Value reply_header;
    reply_header["requestID"] = m_requestID;
    reply_header["type"] = static_cast<int>(m_reply_type);
    m_reply_pkt["header"] = reply_header;

    Json::Value forward_header;
    forward_header["type"] = static_cast<int>(m_forward_type);
    forward_header["senderID"] = m_senderID;
    m_forward_pkt["header"] = forward_header;
}
