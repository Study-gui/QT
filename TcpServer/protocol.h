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

#define DIR_NO_EXIST "dir no exist"
#define FILE_NAME_EXIST "file name exist"
#define CREAT_DIR_OK "creat dir ok"

#define DEL_DIR_OK "delete dir ok"
#define DEL_DIR_FAILURED "delete dir failured: is reguler file"

#define RENAME_FILE_OK "rename file ok"
#define RENAME_FILE_FAILURED "rename file failured"

#define ENTER_DIR_FAILURED "enter dir failured: is reguler file"

#define UPLOAD_FILE_OK "upload file ok"
#define UPLOAD_FILE_FAILURED "upload file failured"

#define DEL_FILE_OK "delete file ok"
#define DEL_FILE_FAILURED "delete file failured: is dir"

#define MOVE_FILE_OK "move file ok"
#define MOVE_FILE_FAILURED "move file failured"
#define COMMON_ERR "operate failed: system is busy"
//存放返回文件信息的结构体
struct FileInfo
{
    char caFileName[32]; //文件名字
    int iFileType;   //文件类型
};


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

    ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_REQUEST,//刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_RESPOND,//刷新好友回复

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,//删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,//删除好友回复


    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,//私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,//私聊回复

    ENUM_MSG_TYPE_GROUP_CHATG_REQUEST,//群聊请求
    ENUM_MSG_TYPE_GROUP_CHATG_RESPOND,//群聊回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,//创建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND,//创建文件夹回复


    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,//刷新文件夹请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND,//刷新文件夹回复

    ENUM_MSG_TYPE_DEL_DIR_REQUEST, //删除文件夹请求
    ENUM_MSG_TYPE_DEL_DIR_RESPOND, //删除文件夹回复

    ENUM_MSG_TYPE_RENAME_FILE_REQUEST, //重命名文件夹请求
    ENUM_MSG_TYPE_RENAME_FILE_RESPOND,//重命名文件夹回复


    ENUM_MSG_TYPE_ENTER_DIR_REQUEST, //进入文件夹请求
    ENUM_MSG_TYPE_ENTER_DIR_RESPOND,//进入文件夹回复

    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST, //上传文件请求
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND, //上传文件回复

    ENUM_MSG_TYPE_DIR_FILE_REQUEST, //删除普通文件请求
    ENUM_MSG_TYPE_DIR_FILE_RESPOND, //删除普通文件回复

    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST, //下载文件请求
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,//下载文件回复

    ENUM_MSG_TYPE_SHARE_FILE_REQUEST, //分享文件请求
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,//分享文件回复

    ENUM_MSG_TYPE_SHARE_FILE_NOTE,//分享文件询问

     ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND, //分享文件询问回复

    ENUM_MSG_TYPE_MOVE_FILE_REQUEST, //移动文件请求
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,//移动文件回复

    // ENUM_MSG_TYPE_REGST_RESPOND,
    ENUM_MSG_TYPE_MAX=0X00FFFFFF
};
struct PDU
{
    uint uiPDULen;      //总的协议数据单元大小

    uint uiMsgType;     //消息类型

    char caData[64];    //用户信息

    uint uiMsgLen;      //实际消息长度

    int caMsg[];       //实际消息//弹性数组，便于利用空间

};

PDU* mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
