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

  template<class T,class Ref,class Ptr>
  struct List_iterator{
    typedef ListNode<T> Node;
    typedef List_iterator<T,Ref,Ptr> self;
    typedef T value_type;
    Node* _node;  //指向list中的每个节点的指针


    //迭代器的构造
    List_iterator(Node* node)
      :_node(node)
    {}

    T& operator*()
    {
      return _node->_data;
    }

    T* operator->()
    {
      return &(operator*());
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
    self operator++()     //前置++
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

  
  template<class T,class Ref,class Ptr,class Iterator>
  struct List_reverse_iterator{
    typedef ListNode<T> Node;
    typedef List_reverse_iterator<T,Ref,Ptr,Iterator> self;
    typedef T value_type;
    Iterator _it;  //传入的一个正向迭代器


    //反向迭代器的构造
    List_reverse_iterator(const Iterator& it)
      :_it(it)
    {}

    Ref operator*()
    {
      return *_it;
    }

    Ptr operator->()
    {
      return &(operator*());
    }


    //反向迭代器的比较
    bool operator==(const self& node)
    {
      return _it == node._it;
    }

    bool operator!=(const self& node)
    {
      return _it != node._it;
    }

    //反向迭代器的++
    self operator++()     //前置++
    {
      --_it;  //相当与正向迭代器的++
      return *this;
    }

    self operator++(int)  //后置++
    {
      self tmp = *this;
      _it--;
      return tmp;
    }

    //迭代器--
    self& operator--()   //前置--
    {
      ++_it;
      return *this;
    }

    self operator--(int)  //后置--
    {
      self tmp = *this;
      _it++;
      return tmp;
    }
};

  template<class T>  //list类
  class List{
    typedef ListNode<T> Node;
    public:
    typedef T value_type;
    typedef List_iterator<T, T& ,T*> iterator;
    typedef List_iterator< T, const T& ,const T*> const_iterator;
    typedef List_reverse_iterator<T, T& ,T*,iterator> reverse_iterator; 
      //迭代器函数
      iterator begin()
      {
        return iterator(_hnode->_next);
      }

      iterator end()
      {
        return iterator(_hnode);
      }

      const_iterator cbegin() const 
      {
        return const_iterator(_hnode->_next);
      }

      const_iterator cend() const
      {
        return const_iterator(_hnode);
      }

      //反选迭代器
      reverse_iterator rbegin()
      {
        return reverse_iterator(iterator(_hnode->_prev));
      }

      reverse_iterator rend()
      {
        return reverse_iterator(iterator(_hnode));
      }

      //构造函数
      List()  //无参构造
      :_hnode(new Node)
      {
        _hnode->_next = _hnode;
        _hnode->_prev = _hnode;
      }

      List(int size, const value_type& value = value_type() )
        :_hnode(new Node)
      {
        _hnode->_next = _hnode;
        _hnode->_prev = _hnode;
        while(size--)
        {
          Push_back(value);
        }
      }

      List( List<value_type>& l)
        :_hnode(new Node)
      {

        _hnode->_next = _hnode;
        _hnode->_prev = _hnode;
        const_iterator it2 = l.cbegin();
        while(it2 != l.cend())
        {
          Push_back(*it2);
          it2++;
        }
                   
      }


      template<class InputIterator>
      List(InputIterator first, InputIterator last)
      {

       _hnode = new Node;
        _hnode->_next = _hnode;
        _hnode->_prev = _hnode;
        while(first != last)
        {
          Push_back(*first);
          first++;
        }
      }


      List<value_type>& operator=( List<value_type>& list)
      {

        List<value_type> tmp(list);
        swap(_hnode,list._hnode);

        return *this;
      }

      ~List()
      {
        Clear();
       delete _hnode;
        _hnode = NULL;
      cout<<"~List"<<endl;

      }
      //容量
      bool Empty()
      {
        return _hnode->_next == _hnode ;
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

        return iterator(newnode);   
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
        Node* next = pos._node->_next;
        Node* prev = pos._node->_prev;
        Node* cur = pos._node;
        iterator tmp = cur->_next;

        next->_prev = prev;
        prev->_next = next;

        DestoryNode(cur);

        return iterator(tmp);
        
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
       Node* _hnode;

  };
}



#endif
