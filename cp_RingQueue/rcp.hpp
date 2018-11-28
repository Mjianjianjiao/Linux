#ifndef __RINGQUEUE_HPP__
#define __RINGQUEUE_HPP__ 



#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//使用posix信号量实现生产者消费者模型
//使用循环队列来作为生产者与消费者的交易场所
template<class T>
class RingQueue{
  public:
    RingQueue(const int& cap)
      :rq(cap),
      _cap(cap),
      p_index(0),
      c_index(0)
    {
      sem_init(&blank,0,cap);   //信号量的不同初始值保证了，生产者先开始，保证了同步
      sem_init(&data,0,0);
      pthread_mutex_init(&lock,nullptr);
    }

    ~RingQueue()
    {
      sem_destroy(&blank);
      sem_destroy(&data);
      pthread_mutex_destroy(&lock);
    }

   void Push_data(const T& value)
    {
      P_blank();   //申请空格
      LockQueue();
      p_index %= _cap;
      rq[p_index++] = value;
      UnLockQueue();  
      V_data();  //数据信号量加1     //保证了同步操作
      
    }

   void Pop_data( T& value)
    {
      P_data();
      LockQueue();
      c_index %= _cap;         //循环队列要对下标取值
      value = rq[c_index++];
      UnLockQueue();
      V_blank();
    }

  private:
   void P_blank()    //申请空格信号量
    {
      sem_wait(&blank);
    }

   void V_blank()   //归还空格信号量
   {
     sem_post(&blank);
   }

   void P_data()   //申请数据信号量
   {
     sem_wait(&data);
   }

   void V_data()  //归还数据信号量
   {
     sem_post(&data);
   }

   void LockQueue()
   {
     pthread_mutex_lock(&lock);
   }

   void UnLockQueue()
   {
     pthread_mutex_unlock(&lock);
   }

  private: 
    sem_t blank;  //空格的信号量，来表示
    sem_t data;   //存入数据的信号量
    pthread_mutex_t lock;  //在有多个生产者与消费者时用锁保护
//两个信号量分别来保证生产者与消费者的同步
    std::vector<T> rq;  //将其作为一个循环队列进行存取数据
    int _cap;  //该队列的容量

    int p_index;  //生产者走的位置
    int c_index;  //消费者走的位置
//消费者与生产者不会从同一个位置上操作，从而实现了互斥，保证消费者在生产者的后面



};



#endif


























