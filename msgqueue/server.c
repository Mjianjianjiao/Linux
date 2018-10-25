#include "common.h"

int main()
{


 int msgid=creatMsgque();

 char buf[1024];

 while(1)
 {
     buf[0]=0;  
    recvMsg(msgid,buf,CLIENT_TYPE);
      if(strcmp(buf,"exit")==0)
      {
        printf("client quit\n");
        break;
      }
    printf("client # %s \n",buf);
    printf("please enter # ");
    fflush(stdout);
    ssize_t s=read(0,buf,sizeof(buf)-1);
    if(s>0)
    {
      buf[s-1]=0;//消除回车并以'\0’结尾
      sendMsg(msgid,buf,SERVER_TYPE);
    }
    else if(s<0)
    {
      perror("sendmsg error");
      break;
    }
 }
   destoryMsg(msgid); 
  return 0;
}
