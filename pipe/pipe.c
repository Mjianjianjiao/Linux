#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

  int fds[2];
  if(pipe(fds)<0)
  {
    perror("pipe");
    exit(1);
  }

  pid_t id=fork();

  if(id==0)
  {
    //child r
    close(fds[0]); //fds[0]是读端，在子进程发送消息要写入
  const  char*  buf="i am child!";
     int i=5; 

     while(i)  //写五次
     {
      write(fds[1],buf,strlen(buf));
      printf("child:已发送\n");
      i--;
      sleep(1);
     }

     close(fds[1]);
     exit(1);
  }else if(id>0){
    //parent w
    close(fds[1]);//fds[1]是写端，父进程接收数据
   char*  buff[1024];
    while(1)
    {
      ssize_t s=read(fds[0],buff,sizeof(buff)-1); //预留一个字节存放‘/0'

      if(s>0)
      {
         buff[s]=0;
         printf("parent:%s\n",buff);
      }
      if(s==0)
      {
        printf("child quit ! parent quit!");
        
      break;
      }

    }

    close(fds[0]);

  }else{  
    perror("fork");
    exit(1);
  }

   
  return 0;
}
