#include <iostream>
#include <pthread.h>
#include <unistd.h>
const int pth_num = 5;
int number = 0;
pthread_rwlock_t rwlock;
void* write_routine(void* arg)
{

  while(1)
  {
    usleep(1000);
    pthread_rwlock_wrlock(&rwlock);
    std::cout<<"write change the number: "<<++number<<std::endl;
    pthread_rwlock_unlock(&rwlock);
  }

}

void* read_routine(void* arg)
{
  int i = *(int*)arg;
  while(1)
  {
    pthread_rwlock_rdlock(&rwlock);
    std::cout<<"reader "<<i<<"get the number: "<<number<<std::endl;
    pthread_rwlock_unlock(&rwlock);
    usleep(300);
  }

  delete (int*)arg;
}

int main()
{

  pthread_t id[pth_num];

  pthread_rwlock_init(&rwlock,NULL);


  pthread_create(&id[0], NULL, write_routine, NULL);
  for(int i = 1; i < pth_num; i++)
  {
      int* p = new int;
      p = &i;
      pthread_create(&id[i], NULL, read_routine, (void*)p);
      usleep(550);
  }

  for(int i = 0; i < pth_num; i++)
  {
    pthread_join(id[i], NULL);
  }

  pthread_rwlock_destroy(&rwlock);
  return 0;
}
