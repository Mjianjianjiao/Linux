#include <iostream> 
#include "Share_ptr.hpp"
using std::cout;
using std::endl;

int main(){

  Shared_ptr<int> sp1(new int);
  *sp1 = 10;
  
  cout << "refcount1" << sp1.Get() << endl;
  {  Shared_ptr<int> sp2(sp1);
  *sp2 = 20;
  }

 // cout << *sp2 << endl;
  cout << "refcount 21  " << sp1.Get() << endl;
  cout << *sp1 << endl;


}
