#include "common.h"

static int commonsqueue(int flags)//将该函数设置为静态外部不可见
{

  key_t _key=ftok(PATHNAME,PROJ_ID);  //生成key值
 if(_key<0)
 {
  perror("ftok error");
  exit(1);
 }
  
 int msgid=msgget(_key,flags|0644);  //创建一个消息队列
  if(msgid<0)
  {
    perror("msgid error");
    return -1;
  }


 return msgid;  //返回创建队列后的识别码

}

int creatMsgque()  //创建消息队列
{
   return commonsqueue(IPC_CREAT|IPC_EXCL);  
  
}
int getMsgque()
{
  return commonsqueue(IPC_CREAT);//仅用于打开一个消息队列
}//打开消息队列
int destoryMsg(int msgid)//删除一个消息队列
{

   if(  msgctl(msgid,IPC_RMID ,NULL ) <0) //第三个参数表示不需要队列的属性信息
   {
     perror("delete error");
     return -1;
   }
}


int sendMsg(int msgid,const void* msgp,int type)//向消息对列中添加消息
{

  struct msgbuf buf;
  buf.mtype=type;
  strcpy(buf.mtext,msgp);
  if( msgsnd (msgid,(void*)&buf,sizeof(buf.mtext),0)<0)
  {
    perror("senmsg error");
    return -1;
  }
}




int recvMsg(int msgid,void* str,int type)//从消息队列中取消息
{
  struct msgbuf buf;
  if(msgrcv(msgid,(void*)&buf,sizeof(buf.mtext),type,0 )<0)
  {
    perror("receive msg error");
    return -1;
  }
  strcpy(str,buf.mtext);
  return 0;
}

