#include "mylist.h"

      template<class value_type>
      void mylist::List<value_type>::Clear()
      {   
        while(_hnode->_next != _hnode)
        {
          Pop_back();
        }

      }

      template<class value_type>
      void mylist::List<value_type>::Remove(const value_type& value)
      {
        iterator it = begin();
        while(it != end())
        {
          if(*it == value)
          {
            it = Rrase(it);
          }
          else it++;
        }
      }

     template<class value_type>
     void mylist::List<value_type>::Unique()
     {
       iterator it = begin();
       iterator last = --end();
       while(it != last)
       {
         if(*it == *(++it))
         {
           it = Erase(--it);
         }
         
       }
    }
void Test1()
{
   mylist::List<int> list;
    cout<<list.Size()<<endl;

   mylist::List<int> list2(10,19);
   mylist::List<int>::iterator it = list2.begin();
   while(it != list2.end())
   {
     cout<<*it<<" ";
     it++;
   }
   cout<<endl;
   cout<<list2.Size()<<endl;
   
   int arr[]={1,2,3,4,5,6};

   mylist::List<int> list3(arr,arr+sizeof(arr)/sizeof(int));
   mylist::List<int>::iterator it3 = list3.begin();
   while(it3 != list3.end())
   {
     cout<<*it3<<" ";
     it3++;
   }
   cout<<endl;


   mylist::List<int> list4(list3);
   mylist::List<int>::iterator it4 = list4.begin();
   while(it4 != list4.end())
   {
     cout<<*it4<<" ";
     it4++;
   }

   
   mylist::List<int> list5;
   list5 = list4;
   it4 = list5.begin();
   while(it4 != list5.end())
   {
     cout<<*it4<<" ";
     it4++;
   }
    cout << endl;
}

void Test2()
{

   cout<<"222"<<endl;
   int arr[]={1,2,2,5};

   mylist::List<int> list2(19,10);
//   mylist::List<int> list3(list2);
mylist::List<int> list3(arr,arr+sizeof(arr)/sizeof(arr[0]));
   

//   mylist::List<int> list3(4,10);
//   list3.Clear();

   
   cout<<"333"<<endl;
   mylist::List<int>::reverse_iterator it3 = list3.rbegin();
   
//  list3.Erase(list3.begin());
  // list3.Pop_back();
   //list3.Push_front(4);  

   //list3.Pop_front();
  // list3.Remove(2);
  // list3.Clear();
  cout<<"1111"<<endl;
  list3.Unique();
//  list3.Clear();
   it3 = list3.rbegin();
   while(it3 != list3.rend())
   {
     cout<<*it3<<" ";
     it3++;
   }
 
}



int main()
{
 // Test1();
  Test2();
  return 0;
}
