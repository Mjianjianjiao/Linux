#ifndef __STRING_H__
#define __STRING_H__ 
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
#define npos -1
namespace myString{

class String{ 
 friend ostream& operator<<(ostream& out,String& s);
 friend istream& operator>>(istream& _cin,String& s); 
  public:
   friend ostream& operator<< (ostream& out,String & s);
    //迭代器
    typedef char* iterator;
    typedef char* const_iterator;
    iterator begin()
    {
      return _str; 
    }
    iterator end()
   {
     return _str+_size;
   }
    String(const char* str="")//构造函数
      :_str(new char[strlen(str)+1])
    {
        strcpy(_str,str);
        _size=strlen(str);
        _capacity=_size;
    }
   ~String()//析构函数
    {
     delete[] _str;
      _size=0;
      _capacity=0;
    }
   String(const String& s)//拷贝构造
     :_str(nullptr),
     _size(0),
     _capacity(0)
    {
       String tmp(s._str);
       this->Swap(tmp);
    }
    
   //string交换
    void Swap(String& s)
    {  
      swap(_str,s._str);
      swap(_size,s._size);
      swap(_capacity,s._capacity);
    }
    //赋值运算符重载
   String& operator=(const String& s)
   {  
     String tmp(s._str);
     swap(_str,tmp._str);
      _size=s._size;
      _capacity=s._capacity;
      return *this;
   }
   const char* C_str()
   {
     return _str;
   }
   size_t size()
   {
     return _size;
   }
  size_t Capacity()
  {
    return _capacity;
  }
   void Reserve(size_t n);
   void Resize(size_t n,char s=0);
  void Push_Back(char ch);
  void Append(const char* str);
  String& operator+= (char ch);
  String& operator+= (const char* str);
  size_t Find(char ch,size_t pos=0);
  size_t Find(char* str ,size_t pos=0);
  void Insert(size_t pos,char ch);
  void Insert(size_t pos,char* str);
  size_t Erase(size_t pos ,size_t len= npos);
  String& assign(char* str);//分配一个新的字符串取代原字符串
  bool Empty()
  {
    if(_size==0)
      return true;
    else return false;
  }
  char  operator[](size_t n)
  {
    return _str[n];
  }
  String operator+(String& s)
  {
    String tmp(*this);
    tmp.Append(s._str);
    return tmp;
  }
 bool operator>(String& s)
 {
    if ( strcmp(_str,s._str)>0)
      return true;
    else return false;
 }
 bool operator<(String& s)
 {
    size_t i=0;
    while(i!=_size)
    {
      if(_str[i]==s._str[i])
        i++;
      else if(_str[i]<s._str[i])
        return true;
      else return false;
    }
    if(i==_size)
      return false;
 }
  private:
    char* _str;
    size_t _size;
    size_t _capacity;
  //  static const size_t npos;
 };

//static const size_t npos=-1;
//
ostream& operator<<(ostream& out,myString::String& s)
{
    out<<s._str;
    return out;
}
istream& operator>>(istream& _cin ,myString::String& s)
{
    _cin>>s._str;
    return _cin;
}
}





#endif 
