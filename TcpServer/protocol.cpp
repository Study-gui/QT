#include"protocol.h"


PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;
    PDU* pdu=(PDU*)malloc(uiPDULen);
    if(NULL==pdu)
    {
        exit(EXIT_FAILURE);
    }
    //清空
    memset(pdu,0,uiPDULen);
    //实际数据大小
    pdu->uiPDULen=uiPDULen;
    //实际消息长度
    pdu->uiMsgLen=uiMsgLen;
    return pdu;
}
