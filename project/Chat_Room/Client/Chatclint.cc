#include "client.h"
#include <stdlib.h>
#include  "window.h"
#include "data.h"
#include <pthread.h>
#include <vector>
#include <signal.h>


volatile int is_quit = 0;

struct thread_arg{
  Client* cl;
  window* wind;
  std::string user_name;
  std::string user_school;
};

struct thread_arg user;
std::vector<std::string> friends;

static void add_user(std::string info){
  std::vector<std::string>::iterator it = friends.begin();
  for(; it != friends.end(); ++it){
    if(*it == info){
      return;
    }
  }

  friends.push_back(info);
}

static void del_user(std::string info){
  std::vector<std::string>::iterator it = friends.begin();
  for(; it != friends.end(); ++it){
    if(*it == info){
      friends.erase(it);
    }
  }
}

void* input_thread(void* arg){
  thread_arg* th_arg = (thread_arg*)arg;
  Client* cli = th_arg->cl;
  window* w = th_arg->wind;
  
  w->draw_input();
  std::string tips = "Please Enter# ";
  std::string message;
  int y, x;
  data d;
  std::string out_string;
  while(1){
    getmaxyx(w->GetInput(), y, x);
    w->PutStringToWindow(w->GetInput(), 1, 2, tips);
    w->GetStringFromWindow(w->GetInput(), message);
    d._name = user.user_name;
    d._school = user.user_school;
    d._type = "null";
    d._message = message;
    d.serialize(out_string);

    cli->send_data(out_string);

    w->draw_input();

  }
}
void* output_thread(void* arg){

  thread_arg* th_arg = (thread_arg*)arg;
  Client* cli = th_arg->cl;
  window* w = th_arg->wind;

  w->draw_output();
  w->draw_friend_list();
  std::string out_string;
  std::string print_string;
  data d;
  int x , y;
  int i = 1 ;
  while(1){
    cli->recv_data(out_string);
    d.unserialize(out_string);
    
    print_string = d._name;
    print_string += "-";
    print_string += d._school;
    if(d._type == "quit")
    {
      del_user(print_string);

    }else{

   add_user(print_string);
  
    print_string += "# ";
    print_string += d._message;
    getmaxyx(w->GetOutput(), y , x);  
    w->PutStringToWindow(w->GetOutput(), i++, 2, print_string);
    if(i >= y - 1){
         i = 1;
      w->draw_output();
    w->PutStringToWindow(w->GetOutput(), i++, 2, print_string);
    }

    }
 
   
    //flist
    w->draw_friend_list();
    int j = 0;
    for(; j < friends.size(); j++){
      w->PutStringToWindow(w->GetFriendlist(), j+1, 2, friends[j]);

    }
  }


}

void* header_thread(void* arg){
  thread_arg* th_arg = (thread_arg*)arg;
  Client* cli = th_arg->cl;
  window* w = th_arg->wind;
  std::string title = "====ChatRoom====";
  int i = 1;
  int y, x;
  int dir = 0;
    while(1){
     w->draw_header();//重新绘制，消除上次的字符
      getmaxyx(w->GetHeader(), y, x);
       w->PutStringToWindow(w->GetHeader(), y/2, i, title);
 
       if(i >=  (x - title.size() - 3)){
             dir = 1;
      }

      if(i <= 4){
        dir = 0;
      }

      if(dir == 0 ){
        i++;
      }else if(dir == 1) 
        i--;

      usleep(33000);
    }


}



void usage(std::string proc){
  std::cout << " usage : " << proc << "ip " << "port" << std::endl;
}


void send_quit(int sig){
  data d;
  d._name = user.user_name;
  d._school = user.user_school;
  d._message = "None";
  d._type = "quit";

  std::string out_string;
  d.serialize(out_string);
  user.cl->send_data(out_string);
  is_quit = 1;
}

int main(int argc, char* argv[]){
  if(argc != 3){
    usage(argv[0]);
    return 1;
  }

  Client cl(argv[1], atoi(argv[2]));
  cl.Init_Client();
  
  std::cout << "please enter name # ";
  std::cin >> user.user_name ;
  std::cout << "please enter school # "<< user.user_school;
  std::cin >> user.user_school ;

  signal(SIGINT, send_quit);

  window win;
 // thread_arg* th_arg = new thread_arg();
  user.cl = &cl;
  user.wind = &win;

  pthread_t input, output, header;
  pthread_create(&header, NULL, header_thread, (void*)&user);
  pthread_create(&output, NULL, output_thread, (void*)&user);
  pthread_create(&input, NULL, input_thread, (void*)&user);

  while(!is_quit){
    sleep(1);
  }
//  pthread_join(header, NULL);
//  pthread_join(output, NULL);
//  pthread_join(input, NULL);

  return 0;
}
