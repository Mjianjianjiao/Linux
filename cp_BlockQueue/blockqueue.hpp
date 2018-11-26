#ifndef __BLOCKQUEUE_HPP__
#define __BLOCKQUEUE_HPP__ 

#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdlib.h>
#include <time.h>


//为生产者与消费者创建一个交易场所
template<class T>
class BlockQueue{
  public:

BlockQueue(const int& cap) 
      :_cap(cap)
    {
      pthread_mutex_init(&lock,NULL);   //      pthread_cond_init(&cond_c,NULL);
      pthread_cond_init(&cond_p,NULL);
    }

    void Push_Data(const T& value)  //放入数据
    { 
      LockQueue();

      while(IsFull())  //判断队列是否为满,函数调用可能失败，使用while进行轮询式检测
        ProductWait();
      SignalConsume();
      bq.push(value);  //不满插入数据

      UnLockQueue();
    }

    void Pop_Data(T& data)  //取出，并从队列中删除数据
    {
      LockQueue();

      while(IsEmpty())  //判断是否为空
        ConsumeWait();

      SignalProduct();
      data=bq.front();   //取出数据
      bq.pop();

      UnLockQueue();

    }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);  //销毁条件变量与互斥锁
      pthread_cond_destroy(&cond_c);
      pthread_cond_destroy(&cond_p);
    }
  private:
    //将对互斥锁和条件变量的操作私有化防止被从类外访问
    void LockQueue()        //上锁
    {
      pthread_mutex_lock(&lock);
    }

    void UnLockQueue()      //解锁
    {
      pthread_mutex_unlock(&lock);
    } 

    void ConsumeWait()      //消费者等待
    {
      pthread_cond_wait(&cond_c,&lock);
    }
 
    void ProductWait()      //生产者等待
    {
      pthread_cond_wait(&cond_p,&lock);
    }

    void SignalConsume()    //通知消费者
    {
      pthread_cond_signal(&cond_c);
    }

    void SignalProduct()   //通知生产者
    {
      pthread_cond_signal(&cond_p);
    }

    bool IsEmpty()         //队列是否为空
    {
      return bq.size() == 0;
    }

    bool IsFull()         //队列是否已满
    {
      return bq.size() == _cap;
    }

  private:
    std::queue<T> bq; //为生产者与消费者建立一个存放数据的队列
    int _cap;  //限定了该队列的大小为cap

    pthread_mutex_t lock;  //互斥锁用于保护临界区
    pthread_cond_t cond_c; //两个条件变量分别对应 队列为空时的 
    pthread_cond_t cond_p; //对应队列为满时

};



#endif 
