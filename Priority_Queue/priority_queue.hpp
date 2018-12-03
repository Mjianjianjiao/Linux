#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__ 

#include <iostream>
#include <vector>
#include <algorithm>
/*
 *优先级队列: 根据出入的第三个模板参数，来确定是大的数为优先级高，还是小的优先级高
 *  pop时 每次pop出队列中最的或最小的数
 *  top时 top出最大或最小的数 
 * */


/*  通过构建堆来完成pop 与 top 的操作。
 *  vector 相当于一颗完全二叉树 ，可以通过下标随机访问，用来构建堆最合适
 * */

template<class T,class Container = std::vector<T>>
class PriorityQueue{  //此时默认为less 大的数优先级高
  public:
    void Push(const T& value)
    {
      _con.push_back(value); //插入到数组后最后一个位置
      Adjustup(_con.size()-1);   //再将数据向上调整重新构成堆
    }

    void Pop()
    {
      std::swap(_con[0],_con[_con.size()-1]);
      _con.pop_back();
      Adjustdown();
    }

    T& top()
    {
      return _con[0];
    }

    void Adjustup(int n)
    {
      int child = n;
      int parent = (child - 1 ) / 2;
      while(child > 0)
      {
        if(_con[parent] < _con[child])
          std::swap(_con[parent],_con[child]);

        child = parent;
        parent = (child - 1 ) / 2 ; 
        
      }
    }

    void Adjustdown()
    {
      int parent = 0;
      int child = parent * 2 + 1;
      while(child < _con.size())
      {
        if(child +1  < _con.size() && _con[child] < _con[child + 1])
          child = child + 1;

        if(_con[parent] < _con[child])
          std::swap(_con[parent] , _con[child]);
        else break;

        parent = child;
        child = parent * 2 + 1;
      }
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


# endif 
