#include "common.h"

int main()
{
   int semid=creatSemset(1);
   initSem(semid,0,1);//初始化信号量级的计数值
   pid_t id=fork();//
   if(id==0)
   {
     //child
    int _semid=getSemSet(1);//参数为该信号量的个数
   while(1){
    P(_semid,0);
     cout<<"a";
     fflush(stdout);//使父子进程向显示器这个临界资源打印，使用二元信号量（互斥锁）来保证父子进程操作的原子性
     usleep(12345);
     cout<<"a ";
     fflush(stdout);
    V(_semid,0);

    }
   }
   else if(id>0)
   {
    
     //parent
   while(1){
     P(semid,0);
     cout<<"b";
     fflush(stdout);
     usleep(23456);
     cout<<"b ";
     fflush(stdout);

     usleep(32465);
     V(semid,0);
     }
   }else{
      perror("fork error");
      exit(1);
   }
  destorySem(semid);
  return 0;
}
