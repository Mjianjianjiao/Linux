#ifndef __COMMON_H_
#define __COMMON_H_ 


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#define PATHNAME "."  //ftok 的路径参数设置为当前路径
#define PROJ_ID   0x666     //ftok 的另一个参数
#define SERVER_TYPE 1
#define CLIENT_TYPE 2 

struct msgbuf
{
  long mtype;
  char mtext[1024];
};

int creatMsgque();  //创建消息队列
int getMsgque();  //打开消息队列
int destoryMsg(int msgid);  //删除一个消息队列
int sendMsg(int msgid,const void* msgp,int type);//向消息对列中添加消息
int recvMsg(int msgid,void* str,int type);
#endif
