#include "client.h"
#include <stdlib.h>
#incldue "window.h"
#include "data.h"
#include <pthread.h>

struct thread_arg{
  Client* cl;
  window* wind;
};

void* input_thread(void* arg){
  
}

void* output_thread(void* arg){

  pthread_detach(pthread_self());

  thread_arg* th_arg = (thread_arg*)arg;
  Client* cli = th_arg->cl;
  window w = th_arg->wind;

  w->draw_output();
  std::string message;
  std::string out_string;
  int x , y;
  int i = 1 ;
  while(1){
    cli->recv_data(out_string);
    getmaxyx(w->GetOutput(), y , x);
    
    w->PutStringToWindow(w->GetOutput(), i, 1, message);

    if(i >= y - 1){
      i = 0;
      w->draw_output();
    }

    i++;
    sleep(1);
  }


}

void* header_thread(void* arg){

}



void usage(std::string proc){
  std::cout << " usage : " << proc << "ip " << "port" << std::endl;
}

int main(int argc, char* argv[]){
  if(argc != 3){
    usage(argv[0]);
    return 1;
  }

  Client cl(argv[1], atoi(argv[2]));
  window win;

  thread_arg* th_arg = new thread_arg();
  th_arg->cl = &cl;
  th_arg->wind = &win;

  pthread_t input, output, header;
  pthread_create(&header, NULL, header_thread, (void*)th_arg);
  pthread_create(&input, NULL, input_thread, (void*)th_arg);
  pthread_create(&output, NULL, output_thread, (void*)th_arg);



  return 0;
}
