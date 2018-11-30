#include "thread_pool.hpp"


int ADD(int x,int y)
{
  return x + y;
}

int SUB(int x,int y)
{
  return x - y;
}




const int PTH_NUM = 100;

int main()
{

  PthreadPool<Task>*  pth = new PthreadPool<Task>(PTH_NUM);

  pth->PthreadPoolInit();   //初始化线程池 
  int count = 0;
  while(1)
  {
   usleep(1000);
    Task t = Task(count,++count,ADD);   
    pth->AddTask(t);    //添加任务

    if(count > 3000)  //线程池的退出条件
    {
      pth->Stop();  
      break;
    }
  }

  delete pth;
  return 0;
}
