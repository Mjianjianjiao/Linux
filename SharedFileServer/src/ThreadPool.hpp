#pragma once 
#include  <iostream>
#include <pthread.h>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include "Utils.hpp"

typedef bool (*Handler)(int sock);
class HttpTask{
    //Http请求处理任务
    //包含一个成员就是socket
    //包含一个任务处理函数
    private:
        int _cli_sock;
        Handler TaskHandler;
    public:
     //  HttpTask(int sock, Handler handle)
		 // 			:_cli_sock(sock)
     //       ,TaskHandler(handle)
		 // 				{}
     //  
       void SetHttpTask(int sock, Handler handle){
         _cli_sock = sock;
         TaskHandler = handle;
       }

       void Run(){
       
         //通过传入函数，降低了耦合度，方便在外部对处理函数进行修改
           TaskHandler(_cli_sock); 
       }
       
       
};
class ThreadPool
{
    //线程池类
    //创建指定数量的线程
    //创建一个线程安全的任务队列
    //提供任务的入队，出队，线程池的销毁、初始化接口
    private:
        int _max_thr;  //线程的最大数量
        int _cur_thr; //当前线程池中的线程数
        bool _is_stop;
        std::queue<HttpTask> _task_queue;  //stl中都是非线程安全的
        pthread_mutex_t _mutex; //解决线程安全
        pthread_cond_t _cond;  //控制同步
    private:
        static void *thr_start(void *arg){
          ThreadPool* tp = (ThreadPool *) arg; 
         
          tp->QueueLock();
          while(tp->QueueIsEmpty()){
            tp->ThreadWait();
          }

          HttpTask ht;
          ht = tp->PopTask();

          tp->QueueUnLock();
          ht.Run();
          return NULL;
        }
    private:
        void QueueLock(){
          pthread_mutex_lock(&_mutex);
        }
        void QueueUnLock(){
          pthread_mutex_unlock(&_mutex);
        }

        bool IsStop()
        {
          return _is_stop;
        }

        void ThreadExit(){
            _cur_thr--;
            pthread_exit(NULL);
          }

        void ThreadWait(){
          
          if(IsStop()){
            QueueLock();
            ThreadExit();
          }

          while(!QueueIsEmpty())
          pthread_cond_wait(&_cond, &_mutex);

        }
        
        void ThreadWakeUpAll(){
          pthread_cond_broadcast(&_cond);
        }

        bool QueueIsEmpty()
        {
          return _task_queue.size() == 0;
        }
      

    public:
        ThreadPool(int max)
          :_max_thr(max)
           , _is_stop(true)
        {}

        ~ThreadPool(){
          pthread_mutex_destroy(&_mutex);
          pthread_cond_destroy(&_cond);
        }

        bool ThreadPoolInit()//完成线程的创建，互斥锁条件变量的初始化
        {
          pthread_t pid;
          for(int i = 0; i < _max_thr; ++i)
          {
            int ret = pthread_create(&pid, NULL, thr_start, this);
            if(ret != 0){
              LOG("thread creat error");
              return false;
            }
          }

          if(!IsStop())
          {
            //若线程要销毁，则无需等待直接销毁即可
            QueueLock();     
            pthread_detach(pid);
            _cur_thr++;
          }
          pthread_mutex_init(&_mutex, NULL);
          pthread_cond_init(&_cond, NULL);
          return true;
        }

        bool PushTask(HttpTask &tt) // 线程安全的任务入队
        {
          QueueLock();
          _task_queue.push(tt);
          QueueUnLock();
          return true;
        }

        HttpTask PopTask() //线程安全的任务出队
        {//因为线程安全的唔唔唔出队是在县城接口中调用的，线程接口中在出对之前就会进行加锁，所以在这里就不许要加索
          HttpTask tt = _task_queue.front();
          _task_queue.pop();
          return tt;
        }
        
        bool ThreadPoolStop() //销毁线程池
        {
          if(!IsStop()){
            _is_stop = true;
          }

          while(_cur_thr > 0){
            ThreadWakeUpAll();//可能有线程处于休眠状态
            usleep(1000);
          }

          return true;
        }
        
};
