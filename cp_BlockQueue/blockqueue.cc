#include "blockqueue.hpp"

const int  QUEUE_CAP = 5;

void* consume_routine(void* arg)
{

  BlockQueue<int>* p = (BlockQueue<int>*) arg; 
 
  srand((unsigned long)time(NULL));
  int data;
  while(1)
  {
    data = rand() % 100 +1;
    p->Push_Data(data);
    std::cout<<"product push the data: "<<data<<std::endl;
  }
}

void* product_routine(void* arg)
{

  BlockQueue<int>* p = (BlockQueue<int>*) arg; 

  int data;
  while(1)
  {
     p->Pop_Data(data);
     std::cout<<"consume get the data : "<<data<<std::endl;
  }
}

int main()
{

  pthread_t consume;
  pthread_t product;

  
  BlockQueue<int>* p = new BlockQueue<int>(QUEUE_CAP); //创建一个存放数据的区域


  pthread_create(&consume,NULL,consume_routine,(void*)p);
  pthread_create(&product,NULL,product_routine,(void*)p);

  pthread_join(consume,NULL);
  pthread_join(product,NULL);


  delete p;
  return 0;
}
