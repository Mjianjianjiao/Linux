#include <iostream>
using namespace std;
//只能在栈上为对象开辟空间,不能使用new
class InStack{
  public:
     static InStack GetStack()
    {
        InStack tmp;
        return tmp;
    }
   void  Print()
   {
     cout<<_a<<endl;
   }
  private:
    int _a;
    //构造私有化
    InStack()
      :_a(100)
    {};
    
    operator new(size_t size)=delete;
    operator delete(size_t size)=delete;
}; 
//InStack p;//
//static InStack p5;//全局和静态都将无法调用构造
int main()
{
  InStack p2=InStack::GetStack();
  p2.Print();
 /// InStack *p4=new InStack;
// InStack p3=p2;
// p3.Print();
  return  0;
}
