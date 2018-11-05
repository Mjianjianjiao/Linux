#include <iostream>
using namespace std;
//声明一个类只能在堆上

class HeapType{
  public:
   static  HeapType* GetHeap()  //无法用对象调用该函数，所以将其声明成全局的
    {
        HeapType* tmp=new HeapType; //在堆上开辟一块空间
        return tmp; 

    }

  void  Print()
    {
      cout<<_a<<endl;
    }
  private:
     int _a;
    //私有化构造函数
    HeapType()
    :_a(10)
    {};
    //将拷贝构造私有化
    HeapType(const HeapType& p)=delete;
};

int main()
{
   HeapType* p=HeapType::GetHeap();//因为类中的函数必须使用对象调用，所以要将GetHeap声明成静态成员函数，使其具有在类中具有全局性
   p->Print();
 //  HeapType p2;     调用拷贝构造与构造函数时都会报错
//   HeapType p3(p);
  return 0;
}


