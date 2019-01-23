#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>

#define NUM 10
class data_pool{
  public:
    data_pool(int cap = NUM);
    void put_message( std::string& str);
    void get_message(std::string& str);
    void datapool_init();
    ~data_pool();
  private:
    std::vector<std::string> r_queue;
    int _cap;
    
    sem_t blank_sem;
    sem_t  message_sem;

    int consume_step;
    int product_step;
};
