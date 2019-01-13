#include "server.hpp"
#include <unordered_map>

void Usermanual(){
  std::cout << "please input : ./server port " << std::endl;
}
            


void calculate(request& req, reponse* resq){

  switch(req.singel){
    case '+':
      resq->sum = req.x + req.y;
      break;
    case '-':
      resq->sum = req.x - req.y;
      break;
    case '*':
      resq->sum = req.x * req.y;
      break;
    case '/':
      if(req.y == 0){
        resq->status = 1;
      }else 
        resq->sum = req.x / req.y;
      break;
    case '%':
      resq->sum = req.x % req.y;
      break;
    default:
      resq->status = 2;
      break;
  }

}

int main(int argc, char* argv[]){

  if(argc != 2){
    Usermanual();  
    exit(1);
  }


  Tcp_Server server;
  server.ServerInit();
  server.Start(atoi(argv[1]), calculate);
  return 0;
}
