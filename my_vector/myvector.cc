#include "myvector.h"

int main()
{
 //构造
 my_vector::Vector<string> nums(10,3);
// cout<<nums.Size()<<endl;
// cout<<nums.Capacity()<<endl;
// cout<<nums.Empty()<<endl;
//   nums.Reserve(23);
//   cout<<nums.Capacity()<<endl;
//   nums.Resize(8);
//   nums.Resize(13,9);
//   nums.Resize(34); 
   
//  my_vector::Vector<int>::Iterator it=nums.begin()+3;
   //nums.Push_back(7);
   //nums.Erase(nums.end()-1);
//  it=nums.Insert(it,7);
 //  nums.Erase(it);
   my_vector::Vector<string> nums3;
   nums3 = nums;
   nums3.Print ();
}
