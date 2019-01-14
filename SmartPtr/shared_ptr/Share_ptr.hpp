#include <iostream>
#include <thread>
#include <mutex>

template<class T> 
class Shared_ptr{
  public:
    Shared_ptr(T* ptr = nullptr):_ptr(ptr), _refcount(new int(1)), _lock(new std::mutex){
      if(_ptr == nullptr){
        *_refcount = 0;
      }
    }

    ~Shared_ptr(){
      Realese();
      }

    Shared_ptr(Shared_ptr<T>& sp):_ptr(sp._ptr), _refcount(sp._refcount), _lock(sp._lock){
    //不为空时才增加引用计数
      if(_ptr != nullptr)
        AddrefCount();
    }

    Shared_ptr& operator=(Shared_ptr<T>& sp){
      //判断是否自己给自己赋值
      if(this == &sp){
        Realese(); //释放旧的管理的资源

        _ptr = sp._ptr;
        _refcount = sp._refcount;
        _lock = sp._lock;

        if(_ptr)
          AddrefCount();
      }
    }

    T* operator->(){
      return _ptr;
    }

    T& operator*(){
      return *_ptr;
    }

    int Get(){
      return *_refcount;
    }
  private:
    T* _ptr;  //指向资源的指针
    int* _refcount; //引用计数，开辟在堆上，通过一个指针实现多个对象之间的共享
    std::mutex* _lock; //互斥锁，保护引用计数，使用同一资源的对象使用同一把锁

    void Realese(){
     
      SubrefCount();//引用计数减一
   
      if(_ptr && _refcount == 0){
        delete _ptr;
        delete _lock;
        delete _refcount;
      }

      if(_ptr == nullptr){
        delete _lock;
        delete _refcount;
      }
    }


    void SubrefCount(){
      _lock->lock();
      --(*_refcount);
      _lock->unlock();
    }

    void AddrefCount(){
      
      _lock->lock();
      ++(*_refcount);
      _lock->unlock();
    
    }
};
