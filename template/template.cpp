#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
//
template<class T>
class Vector{
  public:
    Vector(size_t capacity)
      :_pdata(new T[capacity])
       ,_size(0)
       ,_capacity(capacity)
  {};

    void pushback(int key)
    { 
      if(_size==_capacity)
        CheckCapacity();
      _pdata[_size++]=key;
    }
    void CheckCapacity()
    {
       T* tmp=new T[_capacity*2];
       memcpy(tmp,_pdata,sizeof(T)*_capacity*2);
       _capacity*=2;
       delete[] _pdata;
       _pdata=tmp;    
    }
    void popback()
    {
      --_size;
    }
    ~Vector()
    {
       if(_pdata)
         delete[] _pdata;
    }
    void Print()
    {
      for(int i=0;i<_size;i++)
        cout<<_pdata[i]<<" ";
    }
    T* operator[](int n)
    {
      return _pdata[n];
    }
  private:
      T* _pdata;
      int _size;
      int _capacity;
};



int main ()
{
 Vector<int> L(4);
 L.pushback(1);
 L.pushback(2);
 L.pushback(3);
 L.pushback(4);
 L.pushback(4);
 L.pushback(4);
 L.Print();
 Vector<double> S(3);
 S.pushback(5.3);
 S.pushback(6.6);
 S.pushback(7.9);
 S.pushback(8.4);
 S.pushback(9.0);
 S.Print();
}
