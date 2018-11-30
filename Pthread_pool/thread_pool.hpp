#ifndef __THREADPOOL_HPP__ 
#define __THREADPOOL_HPP__ 

#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>

typedef int (*cal_t)(int,int);   //定义一个函数指针

class Task{   //任务类线程所要执行的任务

  private:
    int x;        //所执行任务的参数
    int y;
    cal_t handle_task;   //接受传入的函数
    int z;          //存放运行结果
  public:
    Task(int& data1,int& data2,cal_t handle)
      :x(data1),
      y(data2),
      handle_task(handle)
    {}

    int Run()
    {
      z = handle_task(x , y);  //运行结果
    }

   void Print()
    {//test 
      std::cout<<"the thread "<<pthread_self()<<" task sequence: "<<z<<std::endl; 
    }
    ~Task()
    {}

};


template<class T>
class PthreadPool{
  private:
    int pth_num;
    std::queue<T> task;
    pthread_cond_t cond;
    pthread_mutex_t lock;
    bool is_stop;

  private:
    static void* thread_routine(void* arg)  
    {//线程调用的函数只能有一个参数，所以将其声明为static函数没有隐含的this指针，将this指针作为参数传入
      PthreadPool<T>* pth = (PthreadPool<T>*)arg;  //传入线程的this指针

        pthread_detach(pthread_self());  //分离线程，使主线程可以干自己的事
      for(;;)    //使一个线程多次执任务
      {
      
        pth->LockQueue();    
        while(pth->TaskIsEmpty())  //任务队列为空，线程进行等待
        {
          pth->PthreadWait();
        }

        Task t = pth->GetTask();  //获得一个任务进行处理
        pth->UnLockQueue();
        t.Run(); //运行任务
        t.Print();//显示任务结果

      }
    }

   void AwakeOneThread()  //唤醒一个线程
   {
     pthread_cond_signal(&cond);
   }

   void AwakeAllThread()    //唤醒所有线程
   {
     pthread_cond_broadcast(&cond);
   }
     
  public:
    PthreadPool(const int& num)
      :pth_num(num)
    {}

    void PthreadPoolInit()    //初始化线程，创建线程
    {
      pthread_mutex_init(&lock,NULL);
      pthread_cond_init(&cond,NULL);
 
      for(int i = 0; i < pth_num; i++)   //不断创建线程，不超过
      {
        pthread_t id;
        pthread_create(&id, NULL, thread_routine, (void*)this);  //创建线程  
      }

    }
  
   void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }

   void UnLockQueue()
   {
     pthread_mutex_unlock(&lock);
   }

   bool TaskIsEmpty()
   {
     return task.size() == 0;
   }

   void PthreadWait()
   {
     if(is_stop)
     {
       UnLockQueue();   //收到退出信号时，将线程退出，并将线程数--
       pth_num--;
       pthread_exit((void*)NULL); //线程退出
       std::cout<<"thread"<<pthread_self()<<std::endl;
     }

     pthread_cond_wait(&cond,&lock);
   }
   
   void AddTask(Task& t)
   {
     LockQueue();
     if(is_stop)
     {
       UnLockQueue();  //如果要终止线程池便不再被允许添加任务
       return ;
     }
     task.push(t); //添加任务
     AwakeOneThread();  //唤醒一个线程
     UnLockQueue();
   }

   Task GetTask()
   {
     Task tmp = task.front();   //获取一个任务
     task.pop();    //并将任务从任务队列去除
     return tmp;
   }

   void Stop()  //线程池的终止函数
   {
     LockQueue();
     is_stop = true;  //终止线程池，修改状态
     UnLockQueue();

     while(pth_num > 0)   //不断的唤醒
       AwakeAllThread();

    
   }

   
   ~PthreadPool()   //在析构中销毁互斥锁与条件变量
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&cond);
    }

};



















#endif
