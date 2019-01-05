#include "client.hpp"

void UserManual(){
  cout << "use the client : ./client ip port" <<endl;
} 

int main(int argc, char* argv[]){

  if(argc != 3){
    UserManual();
    exit(1);
  }

  Udp_Client user(argv[1],atoi(argv[2]));

  string buf;
  string res;

  while(1){

  cout << "client input # " ;
  cin >> buf;
  user.SendTo(buf);
  user.RecvFrom(&res);
  cout << "server respond # " << res << endl;

  }


  return 0;
}
