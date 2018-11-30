#ifndef __STACK_HPP__
#define __STACK_HPP__ 
#include <iostream>
#include <vector>
#include <list>
//stack底层使用vector或使用list 进行适配，对其进行封装
template<class T , class Container = std::vector<T> >  
class Stack{  
  public:
    void Push(const T& value)
    {
      _con.push_back(value);
    }

    void Pop()
    {
      _con.pop_back();
    }

    T& Top()
    {
      return _con.back();
    }

    size_t Size()
    {
      return _con.size();
    }

    bool Empty()
    {
      return _con.size() == 0;
    }


    
  private:
    Container _con;

};





#endif
