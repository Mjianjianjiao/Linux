#ifndef __LIST_H__
#define __LIST_H__ 
#include <iostream>
#include <algorithm>
#include <assert.h>
using namespace std;
namespace mylist{

  template<class T>   //节点类
  struct ListNode{

    typedef ListNode* point_type;
    point_type _prev;
    point_type _next;
    T _data;

    ListNode(const T& value = T())
      :_prev(nullptr),
      _next(nullptr),
      _data(value)
    {}

    };

  template<class T>
  struct List_iterator{
    typedef ListNode<T> Node;
    typedef List_iterator<T> self;
    typedef T value_type;
    Node* _node;  //指向list中的每个节点的指针


    //迭代器的构造
    List_iterator(Node* node)
      :_node(node)
    {}

    value_type operator*()
    {
      return _node->_data;
    }


    //迭代器的比较
    bool operator==(const self& node)
    {
      return _node == node._node;
    }

    bool operator!=(const self& node)
    {
      return _node != node._node;
    }

    //迭代器的++
    self& operator++()     //前置++
    {
      _node = _node->_next;
      return *this;
    }

    self operator++(int)  //后置++
    {
      self tmp = *this;
      _node = _node->_next;
      return tmp;
    }

    //迭代器--
    self& operator--()   //前置--
    {
      _node = _node->_prev;
      return *this;
    }

    self operator--(int)  //后置--
    {
      self tmp = *this;
      _node = _node->_prev;
      return tmp;
    }

};

  


  template<class T>  //list类
  class List{
    typedef ListNode<T> Node;
    public:
    typedef T value_type;
    typedef List_iterator<T> iterator;
      //迭代器函数
      iterator begin()
      {
        return _node->_next;
      }

      iterator end()
      {
        return _node;
      }

      //构造函数
      List()  //无参构造
      :_node(new Node)
      {
        _node->_next = _node;
        _node->_prev = _node;
      }

      List(int size, const value_type& value = value_type() )
        :_node(new Node)
      {
        _node->_next = _node;
        _node->_prev = _node;
        while(size--)
        {
          Push_back(value);
        }
      }

      List(List<value_type>& l)  //问题1：此处拷贝构造无法使用const
      {
        List<value_type> tmp;
        _node = tmp._node;

        iterator it2 = l.begin();
        while(it2 != l.end())
        {
          Push_back(*it2);
          it2++;
        }
                   
      }


      template<class InputIterator>
      List(InputIterator first, InputIterator last)
      {
        List<value_type> tmp;
        _node = tmp._node;

        while(first != last)
        {
          Push_back(*first);
          first++;
        }
      }


      List<value_type>& operator=( List<value_type>& list)
      {

        List<value_type> tmp(list);
        swap(_node,list._node);

        return *this;
      }

      //容量
      bool Empty()
      {
        return _node->_next == _node ? true : false;
      }

      int Size()
      {
        int count = 0;
        iterator it = begin();
        while(it != end())
        {
          count++;
          it++;
        }

        return count;
      }

      iterator Insert(iterator pos, const value_type value)
      {
        Node* newnode = new Node(value);

        newnode->_next = pos._node;
        newnode->_prev = pos._node->_prev;
        pos._node->_prev->_next = newnode;
        pos._node->_prev = newnode;

        return newnode;   
      }


      void Push_back(const value_type& value) //尾插
      {
        Insert(end(),value);
      }
  
      void Push_front(const value_type& value)   //头插
      {
        Insert(begin(),value);
      }

      void DestoryNode(Node* pl)
      {
        delete pl;
      }
      iterator Erase(iterator pos)
      {   //移除一个节点


        pos._node->_prev->_next = pos._node->_next;
        pos._node->_next->_prev = pos._node->_prev;
        iterator tmp = pos._node->_next;

        DestoryNode(pos._node);

        return tmp;
        
      }

      void Pop_back()
      {
          Erase(--end());
      }

      void Pop_front()
      {
          Erase(begin());
      }      


      void Clear();
      void Remove(const value_type& value);
      void Unique();

    private:
       //一个指向链表头节点的指针
       Node* _node;

  };
}



#endif
