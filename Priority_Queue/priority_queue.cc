#include "priority_queue.hpp"

int main()
{

  PriorityQueue<int> pq;
  pq.Push(4);
  pq.Push(5);
  pq.Push(1);
  pq.Push(0);
  pq.Push(7);


  while(!pq.Empty())
  {
  std::cout<<pq.top()<<" ";
  pq.Pop();
  }
  std::cout<<std::endl;
  
  return 0;
}
