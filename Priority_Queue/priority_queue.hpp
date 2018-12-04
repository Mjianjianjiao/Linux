#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__ 

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
/*
 *优先级队列: 根据出入的第三个模板参数，来确定是大的数为优先级高，还是小的优先级高
 *  pop时 每次pop出队列中最的或最小的数
 *  top时 top出最大或最小的数 
 * */


/*  通过构建堆来完成pop 与 top 的操作。
 *  vector 相当于一颗完全二叉树 ，可以通过下标随机访问，用来构建堆最合适
 * */


/*
 *使用仿函数来调整队列中的优先级
 */

template<class T>
class Less{

  public:
    bool operator()(const T& a1, const T& a2)
    {
      return a1 < a2;
    }
};

template<class T> 
class Greater{
  public:
    bool operator()(const T& a1,const T& a2)
    {
      return a1 > a2;
    }
};

template<class T,class Container = std::deque<T> , class Compare = Greater<T>>
class PriorityQueue{  //此时默认为less 大的数优先级高
  public:
    void Push(const T& value)
    {
      _con.push_back(value); //插入到数组后最后一个位置
      Adjustup(_con.size()-1);   //再将数据向上调整重新构成堆
    }

    void Pop()
    {
      std::swap(_con[0],_con[_con.size()-1]);   //使堆顶元素与最后一个元素交换位置，
      _con.pop_back();  //再将最后一个元素删除
      Adjustdown();     //再从堆顶开始将其调整为堆
    }

    T& top()  //取出堆顶的元素
    {
      return _con[0];
    }

    void Adjustup(int n)  //向上调整
    {
      Compare _com;
      int child = n;
      int parent = (child - 1 ) / 2;
      while(child > 0)  //当调整到堆顶元素时就停止
      {
        if(_com(_con[child] , _con[parent]))
          std::swap(_con[parent],_con[child]);

        child = parent;
        parent = (child - 1 ) / 2 ; 
        
      }
    }

    void Adjustdown()  //向下调整
    {
      Compare _com;
      int parent = 0;
      int child = parent * 2 + 1;
      while(child < _con.size())
      {
        if(child +1  < _con.size() && _com(_con[child + 1] , _con[child]))
          child = child + 1;  //选两个孩子中较大的一个

        if(_com(_con[child] , _con[parent]))  //和双亲节点比较
          std::swap(_con[child] , _con[parent]);
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
