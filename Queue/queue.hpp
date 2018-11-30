#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__ 

#include <iostream>
#include <list>
#include <deque>

template<class T, class Container = std::deque<T>>
class Queue{
//无需构造析构，因为封装的是list或者deque,编译器会自动调用这两个容器的构造析构
  public:

    void Pop()
    {
      _con.pop_front();
    }

    void Push(const T& value)
    {
      _con.push_back(value);
    }

    T& Front()
    {
      return _con.front();
    }

    T& Back()
    {
      return _con.back();
    }

     size_t Size()
    {
      return _con.size();
    }

    bool Empty()
    {
      return _con.empty();
    }

  private:
    Container _con;

};








#endif
