#include "client.hpp"
#include <string.h>
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

      request req;
      memset(&req, 0, sizeof(req));
      std::cout << "client input # " ;
      std::cin >> req.x >> req.singel >> req.y;
      if(std::cin.fail()){
        std::cerr << "input  error" << std::endl;
        std::cin.clear();
        std::cin.ignore(1024, '\n');
      }

      if(!client.SendData(&req)){
        std::cerr << "send data failed " << std::endl;
        continue;
      }

      reponse resp;
      memset(&resp, 0, sizeof(resp));
      if(!client.RecvData(&resp)){
        std::cerr << "recvive data failed" << std::endl;
        break;
      }

      std::cout << "server response # " <<"sum: " <<resp.sum <<"status: " << resp.status <<std::endl;      
    }

    client.Close();

    return 0;
}
