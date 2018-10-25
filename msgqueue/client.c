#include "common.h"

int main()
{
char buf[1024];
   
int msgid=getMsgque();

while(1)
{

   printf("please enter # ");
   fflush(stdout);
   ssize_t s=read(0,buf,sizeof(buf)-1);
   if(s>0)
   {
      buf[s-1]=0;  //消除在发送信息时输入的回车符
      
  sendMsg(msgid,buf,CLIENT_TYPE);
if(strcmp(buf,"exit")==0)
  {
    break;
  }
   }else if(s<0){

     perror("read error\n");
    exit(1);
   }
recvMsg(msgid,buf,SERVER_TYPE);
  printf("server # %s\n",buf);
}
  return 0;
}
