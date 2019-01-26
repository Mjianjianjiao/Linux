#include "data_pool.h"

data_pool::data_pool(int cap):_cap(cap),r_queue(cap){
  datapool_init();
}

void data_pool::datapool_init(){
  consume_step = 0;
  product_step = 0;
  sem_init(&blank_sem, 0, _cap);
  sem_init(&message_sem, 0, 0);
}

void data_pool::put_message(std::string& str){

  sem_wait(&blank_sem);
  r_queue[product_step] = str;
  sem_post(&message_sem);
  product_step++;
  product_step %= _cap;
}

void data_pool::get_message(std::string& str){

  sem_wait(&message_sem);
  str = r_queue[consume_step];
  sem_post(&blank_sem);
  consume_step++;
  consume_step %= _cap;

}

data_pool::~data_pool(){
  sem_destroy(&blank_sem);
  sem_destroy(&message_sem);
}
