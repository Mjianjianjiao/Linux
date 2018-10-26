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
   
   ssize_t fid=open("ismypipe",O_WRONLY);
   if(fid<0)
   {
     printf("open error\n");
      return -1;
   
   }
    
   char str[1024];

    while(1)
    {

      printf("please printf: \n");
      ssize_t s=read(0,str,sizeof(str)-1);
      if(s<0)
      {
        perror("read error");
        exit(1);

    }else if(s>0)
    {
        str[s]=0;
      if(str[0]=='q'&&s==2)
      {
          break;
      }
      if(write (fid,str,sizeof(str)-1)<0)
          {
          perror("write error");
          exit(1);
          }
    
  }
    
    }

  return 0;

    
}
