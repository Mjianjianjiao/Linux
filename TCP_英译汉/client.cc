#include "client.hpp"

void Usermanual(){
  std::cout << "please input :./cilent  ip  port (server address)" << std::endl;
}

int main(int argc, char* argv[]){

  if(argc != 3){
    Usermanual();
    exit(1);
  }

    Tcp_Client client(argv[1], atoi(argv[2]));

    client.ConnectServer();

    while(1){

      std::string buf;
      std::cout << "client input # " ;
      std::cin >> buf;

      if(!client.SendData(buf)){
        std::cerr << "send data failed " << std::endl;
        continue;
      }

      std::string resp;
      resp.clear();
      if(!client.RecvData(&resp)){
        std::cerr << "recvive data failed" << std::endl;
        break;
      }

      std::cout << "server response # " << resp << std::endl;      
    }

    client.Close();

    return 0;
}
