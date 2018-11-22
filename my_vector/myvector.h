
#ifndef __myvector_h__ 
#define __myvector_h__ 
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
using namespace std;
namespace my_vector{

  template<class T>
   class Vector{
     public:
///////////////////////////////////////////////////////////////////////////////
       typedef T* Iterator;
       typedef const T* constiterator;
      
       Iterator begin()
       {
         return _start;
       }

       Iterator end()
       {
         return _finish;
       }
       
       constiterator cbegin()
       {
         return _start;
       }
       
       constiterator cend()
       { 
         return _finish; 
       }
/*
 * 容量 
 */    
      size_t Size()
      {
        return _finish-_start;
      }

      size_t Capacity()
      {
        return _endofstorage-_start;
      }

      bool Empty()
      {
        return Size()==0?true:false;
      }

      void Reserve(size_t n)
      {
        if(n>Capacity())
        {
          T* tmp=new T[n];//开辟新的空间
          size_t size=Size();
         if(_start){
          for(int i=0;i<Size();i++)
               tmp[i]=_start[i];  // 交换进行深拷贝(当T为string时需要深拷贝)
         }
          _start=tmp;       //此时为tmp开辟的空间  
          _finish=_start+size;
          _endofstorage=_start+n;
    
        }
      }

      void Resize(size_t n, T value=0)
      {
         if(n<=Size())
         {
           _finish=_start+n;
         }
         else if(n <= Capacity() && n > Size())
         {
           n -= Size();
           while(n--)
           {
             Push_back(value);
             _finish++;
           }           
         }
         else if(n > Capacity())
        {
          Reserve(n);
         while(_finish != _start+n)
         {
           Push_back(value);
         }
        
        }
      }

      Iterator Erase(Iterator pos)
      {
        if(pos+1!=end())
        {
          Iterator begin=pos+1;
          while(begin!=_finish-1)
          {
            *(begin-1)=*begin;
              begin++;
          }

        }
        --_finish;

        return pos;
      }




     
/*********************************************************************
 * 
 *构造函数
 * 
  */
       Vector()  //无参构造
         :_start(nullptr),
          _finish(nullptr),
          _endofstorage(nullptr)
     {}

       Vector(int n,const T value= T())//使用元素个数和数值进行初始化
         :_start(nullptr),
         _finish(nullptr),
         _endofstorage(nullptr)
       {
          Reserve(n);//开辟n个空间

          while(n--)
          {
            Push_back(value);
          }
       }

      //使用任意模板，可以使参数为任意容器的迭代器，如果使用iterator的话只能为vector的容器
         template<typename InputIterator>
         Vector(InputIterator first,InputIterator last)
        {
            Reserve(last-first);//开辟一个迭代器两端大小的空间//
            while(first!=last)
            {
              Push_back(*first);
              first++;
            }
  
        }

       Vector( Vector<T>& v)
         :_start(nullptr),
          _finish(nullptr),
          _endofstorage(nullptr)
       {
  
         Reserve(v.Capacity());
 
         Iterator it=v.begin();
         Iterator it1=begin();
         while(it!=v.end())
         {
           *it1++=*it++;
         }

         _finish=_start+v.Size();
         _endofstorage=_start+v.Capacity();

       }


       void Swap(Vector<T>& v)
       {
         swap(_start,v._start);
         swap(_finish,v._finish);
         swap(_endofstorage,v._endofstorage);
       
       }

       Vector<T> operator=(Vector<T> v)
       {
         this->Swap(v);
         return *this;
       }

      T operator[](size_t i)
      {
        return _start[i];
      }
/*
 * 
 * 
 * 
 * 
*/
    Iterator Insert(Iterator pos,T value)
    {
      assert(pos<=_finish);
      
      size_t offset=pos-_start;//增容更换空间后pos处的迭代器会失效，记录其偏移量

      if(_finish == _endofstorage)
      {
        size_t newCapacity = Capacity()==0?2:2*Capacity(); //防止第一次为空
        Reserve(newCapacity);
      }

      pos = begin() + offset;
      Iterator end =this->end();
      while(end != pos)
      {
        *end = *(end-1);
         end--;
      }

      *pos = value;
      _finish++;
      return pos;

    }

    void Push_back(T v)
    {
      Insert(_finish,v);
    }

    void Pop_back()
    {
      Erase(end()-1);
    }


    void Print()
    {
      for(int i=0;i<Size();i++)
      cout<<_start[i]<<" ";
      cout<<endl;
    }
     
     private:
       T* _start;
       T* _finish;
       T* _endofstorage;
   };


};
                                    
#endif



