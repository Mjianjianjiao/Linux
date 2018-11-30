#include "stack.hpp"

int main()
{

  Stack<int,std::list<int>> st;
  

  st.Push(1);
  st.Push(2);
  st.Push(3);
  st.Push(4);


  while(!st.Empty())
  {
    std::cout<<st.Top()<<" "<<std::endl;
    st.Pop();
  }

  return 0;
}
