#include "server.h"
#include <stdlib.h>
#include <pthread.h>


void useage(std::string proc){
  std::cout << "usage : " << proc << "port " << std::endl;
}


void* recv_message(void* arg){

  Server* sp = (Server*)arg;
  std::string message;
  for(;;){
    sp->recv_data(message);
    std::cout << "debug " << message << std::endl;
  }
}

void* send_message(void* arg){
  Server* send = (Server*)arg;
  std::string message;
  for(;;){
    send->broadcast();
  }
}

int main (int argc, char* argv[]){
  if(argc != 2){
    useage(argv[0]);
    return -1;
  }

  Server server(atoi(argv[1]));
  server.Init_Server();

  pthread_t recv, send;
  pthread_create(&recv, NULL, recv_message, (void*)&server );
  pthread_create(&send, NULL, send_message,(void*)&server);

  pthread_join(recv, NULL);
  pthread_join(send, NULL);
}
