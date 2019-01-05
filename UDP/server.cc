#include "server.hpp"

void handel(string& buf, string* res){
  *res = buf;
}

void Usermanual(){
  cout << " use the server : ./server ip port " << endl;
}

int main(int argc, char* argv[]){

  if(argc != 3){
    Usermanual();
    exit(1);
  }

  Udp_Server server;

  server.Strat(argv[1], atoi(argv[2]), handel);
  return 0;
}
