#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{

  char cmd[1024];
  char* myargv[16];
  int  fd=-1;
  while(1)
{
   
   printf("[wens@localhost myshell]# ");

   fgets(cmd,sizeof(cmd),stdin);
   
   int i=0;
   
   cmd[strlen(cmd)-1]='\0';
  
   myargv[i++]=strtok(cmd," ");//将字符串分割解析
   
   char *ret=NULL;
   
   while(ret=strtok(NULL," "))
   {
      myargv[i++]=ret;
   }
  
   myargv[i]=NULL; 
   
//  for( j=0;j<i;j++)
//     printf("%s\n",myargv[j]);
  
   
   
   


   int j=0;
   pid_t id=fork();
   if(id==0)
  {   
 //   printf("child\n");
   
     for(j=0;j<i;j++)
     {
     if(strcmp(myargv[j],">")==0)  //
     {
        close(1);
        fd=open(myargv[j+1],O_WRONLY|O_CREAT,0644);
 //srv  printf("%d\n",fd);
       if(fd<0)
       {
         perror("open");
         exit(1);
       }
       myargv[j]=NULL;
       i=j;
      break;
     } 
     else if(strcmp(myargv[j],"<")==0)
     {
       close(0);
        fd=open(myargv[j+1],O_RDONLY,0644);
       if(fd<0)
       {
         perror("open");
         exit(1);
       }

       myargv[j]=NULL;
       i=j;
      break;
     }   else if(strcmp(myargv[j],">>")==0)
     {
       close(1);
       fd=open(myargv[j+1],O_WRONLY|O_CREAT|O_APPEND,0644);
       if(fd<0)
       {
         perror("open");
         exit(1);
       }
     
       myargv[j]=NULL;
       i=j;
      break;
     }

     }
       execvp(myargv[0],myargv);
         //进行程序替换
        
       exit(1);
   
    }else{
  //    printf("parent\n");
     waitpid(id,NULL,0);  //等待子进程
        }
}

  
return 0;

}
