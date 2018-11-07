#include "common.h"
int commonSemSet(int nums,int flags)
{
  key_t key=ftok(PATHNAME,PROJ_ID);
    if(key<0)
    {
      perror("error ftok");
      exit(1);
    }
    int semid=semget(key,nums,flags);
    if(semid<0)
    {
      perror("error semget");
      exit(1);
    }
    return semid;
}

int creatSemset(int nums)
{
    return commonSemSet(nums,IPC_CREAT|IPC_EXCL|0666);
}
int getSemSet(int nums)
{
    return commonSemSet(nums,IPC_CREAT|0666);
}
int initSem(int semid,int nums,int initval)//使用初始化设置信号量的计数值
{
    Semnum semnu;
    semnu.val=initval;    //信号量的计数值
    if(semctl(semid,nums,SETVAL,semnu)<0)
    {
      perror("init error");
      exit(1);
    }
    return 0;
}


int commonPV(int semid,int who,int op)
{
    struct sembuf _sem;
    _sem.sem_num=who;  //信号量集在底层是按数组存储，sem_num相当于数组的下标
    _sem.sem_op=op;   //对于信号量的操作一般为 -1 or +1
    _sem.sem_flg=0;
    
    if(semop(semid,&_sem,1)<0)
    {
      perror("pv error");
      exit(1);
    }
     
   return 0;
}
int P(int semid,int who)
{
 commonPV(semid,who,-1);
 return 0;
}
int V(int semid,int who)
{
  commonPV(semid,who,+1);
  return 0;
}
int destorySem(int semid)
{
  if(semctl(semid,0,IPC_RMID)<0)
  {
    perror("destory error");
    exit(1);
  }
  return 0;
}
