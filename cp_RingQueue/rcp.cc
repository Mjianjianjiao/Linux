#include "rcp.hpp"

const int num = 5;

void* consume_routine(void* arg)
{
  RingQueue<int>* rq = (RingQueue<int>*)arg;
  int data;
  while(1)
  {
    rq->Pop_data(data);
    std::cout<<"consume get the data: "<<data<<std::endl;
  }
}

void* product_routine(void* arg)
{
  RingQueue<int>* rq = (RingQueue<int>*)arg;
  srand((unsigned long)time(nullptr));
  int data = 0;
  while(1)
  {
     data = rand() % 100 + 1;
     rq->Push_data(data);
     std::cout<<"product push the data: "<<data<<std::endl;
     sleep(1); //改变生产者的放数据的速度
  }
}


int main()
{
  pthread_t consume;
  pthread_t product;

  RingQueue<int>* rq = new RingQueue<int>(num);
 
  pthread_create(&consume,nullptr,consume_routine,(void*)rq);
  pthread_create(&product,nullptr,product_routine,(void*)rq);

  pthread_join(consume,nullptr);
  pthread_join(product,nullptr);

  delete rq;
  return 0;
}
