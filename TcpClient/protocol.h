#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<stdio.h>
typedef unsigned int uint;
//枚举，用户信息状态
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST, //注册请求
    ENUM_MSG_TYPE_REGST_RESPOND,  //注册回复
    // ENUM_MSG_TYPE_REGIST_REQUEST,
    // ENUM_MSG_TYPE_REGST_RESPOND,
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
