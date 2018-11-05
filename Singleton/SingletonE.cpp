
#pragma GCC diagnostic error "-std=c++11"
#include <iostream>
#include <mutex>
using namespace std;
//饿汉模式:在程序开始之前就创建一个实例化对象

class SingletonE{
  public:
   static SingletonE* GetInstance()//将其声明为一个静态，以便不用创建对象也可以访问
    {  
        return &_Instance;
    }
  private:
    //构造私有化
    SingletonE()
    {
      cout<<"Singletong:饿汉模式"<<endl;
    };
    //拷贝构造私有化
    SingletonE(const SingletonE& pt)
    {};
    SingletonE operator=(const SingletonE& p)
    {};

    static SingletonE _Instance;//在类创建时就已将创建了一个静态的成员变量，只需要在需要时直接调用即可
};

SingletonE SingletonE::_Instance;

class SingletonL{
  public:
    static SingletonL* GetpInstance()
    {
      if(_pInstance==NULL) //双检查，防止其他进程多次加锁，解锁降低程序的效率
       {  

       m_mtx.lock();//加锁  保护线程安全，防止两线程并行时创建两个实例化对象
      if(_pInstance==NULL)  //判断是否是第一次创建该对象
        { 
            _pInstance=new SingletonL;
        return _pInstance;
        }
       m_mtx.unlock();//解锁
      }
    }




    class CGarbo{
      public:
        ~CGarbo(){
           if(_pInstance!=NULL)
             delete _pInstance;
        }
      private:
    };
    
  static CGarbo Garbo;  //定义一个全局的静态变量，在程序结束时，系统会调用他的析构函数，从而会将实例化对象释放
  private:
    SingletonL()
    {
      cout<<"懒汉模式"<<endl;
   };
    SingletonL(const SingletonL& pt)=delete;
    SingletonL operator= (const SingletonL& p)=delete; 
    static SingletonL* _pInstance;
    static mutex m_mtx;//定义一个互斥锁
};

SingletonL*  SingletonL::_pInstance=NULL;
mutex SingletonL::m_mtx;
SingletonL::CGarbo Garbo;
int main()
{
  SingletonE* p=SingletonE::GetInstance();
  SingletonL* p2=SingletonL::GetpInstance();
  return 0;
}




