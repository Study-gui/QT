#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<stdio.h>
typedef unsigned int uint;
#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed: name existed"

#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed: name error or pwd error or relogin"
#define SEARCH_USR_NO "no such people"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"


#define UNKNOW_ERROR "unknow error"
#define EEXISTED_FRIEEND  "friend exist"
#define ADD_FRIEND_OFFLINE "usr offline"
#define ADD_FRIEND_ONLINE "usr not exist"


//枚举，用户信息状态
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST, //注册请求
    ENUM_MSG_TYPE_REGST_RESPOND,  //注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,  //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,   //登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST, //查看在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND, //查看在线用户回复

    ENUM_MSG_TYPE_SEARCH_USR_REQUEST, //查找用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND, //查找用户回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST, //添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,  //添加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGGREE_REQUEST, //添加好友同意请求
    ENUM_MSG_TYPE_ADD_FRIEND_REEUSE_RESPOND, //添加好友不同意请求
    // ENUM_MSG_TYPE_REGST_RESPOND,
    ENUM_MSG_TYPE_MAX=0X00FFFFFF
};

struct PDU
{
    uint uiPDULen;      //总的协议数据单元大小

    uint uiMsgType;     //消息类型

    char caData[64];    //文件名

    uint uiMsgLen;      //实际消息长度

    int caMsg[];       //实际消息//弹性数组，便于利用空间

};

PDU* mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
