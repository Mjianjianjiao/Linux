#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int main()
{
  if(mkfifo("ismypipe",0644)<0)//在一方创建一个命名管道文件
  {
    printf("mkfifo error\n");
    return -1;
  }
   
   ssize_t fid=open("ismypipe",O_RDONLY);   //打开管道文件
   if(fid<0)
   {
     printf("open error\n");
      return -1;
   
   }
    
   char buf[1024];

    while(1)
    {
      ssize_t s=read(fid,buf,sizeof(buf)-1);//从管道文件中读取数据
      if(s<0)
      {
        perror("read error");
        exit(1);

    }else if(s>0)
    {
      buf[s]=0;             
      printf("client# ");
      fflush(stdout);     
      printf("%s",buf);    //将读到的数据从标准输出打印
    }
    else if(s<0)
    {
      perror("read error");
      exit(1);
    }else if(s==0)
    {
      printf("client quit\n");  //如果读到结尾退出
      exit(1);
}
}
close(fid);
  return 0;
}
