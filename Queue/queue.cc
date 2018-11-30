#include "queue.hpp"

int main()
{
  Queue<int,std::list<int>> q;


  q.Push(1);
  q.Push(2);
  q.Push(3);
  q.Push(4);
  q.Push(5);

  std::cout<<q.Front()<<" "<<q.Back()<<std::endl;

  while(!q.Empty())
  {
    std::cout<<q.Front()<<" ";
    q.Pop();
  }
  std::cout<<std::endl;
  return 0;
}
