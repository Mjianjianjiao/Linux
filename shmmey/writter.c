#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int main()
{

   int key=ftok(".",0x6666);
  if(key<0)
  {
     perror("key false");
     return -1;
  }

  int shmid=shmget(key,4096,IPC_CREAT|IPC_EXCL|0666);
  if(shmid<0)
  {
    printf("error \n");
     return -1;
  }

char*  buf=(char*)shmat(shmid,NULL,0);
  int i=0;
 memset(buf,0,4096);
  while(i<30)
  {
     buf[i]='a'+i;
     i++;
  }
   sleep(30);
  shmdt(buf);
  shmctl(shmid,IPC_RMID,NULL);
  return 0;
}



