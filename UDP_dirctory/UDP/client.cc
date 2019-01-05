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
  bool ret = user.RecvFrom(&res);
  if(ret == false){
    cout << "server no response !" << endl;
  }
  cout << "server respond # " << res << endl;

  }


  return 0;
}
