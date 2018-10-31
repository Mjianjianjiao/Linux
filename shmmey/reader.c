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

  int shmid=shmget(key,4096,IPC_CREAT|0666);
  if(shmid<0)
  {
    printf("error \n");
     return -1;
  }


  char*  buf=(char*)shmat(shmid,NULL,0);
  while(1)
  {
    printf("%s",buf);
  }

  shmdt(buf);
  return 0;
}



