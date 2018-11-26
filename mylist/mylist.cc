#include "mylist.h"

      template<class value_type>
      void mylist::List<value_type>::Clear()
      {   
        while(_node->_next != _node)
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
            it=Erase(it);
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

   int arr[]={1,2,2,3,3,2,3,5,5,5,6,5,5};
   mylist::List<int> list3(arr,arr+sizeof(arr)/sizeof(int));
   mylist::List<int>::iterator it3 = list3.begin();
   
//  list3.Erase(list3.begin());
  // list3.Pop_back();
   //list3.Push_front(4);  

   //list3.Pop_front();
  // list3.Remove(2);
  // list3.Clear();
  list3.Unique();
   it3 = list3.begin();
   while(it3 != list3.end())
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
