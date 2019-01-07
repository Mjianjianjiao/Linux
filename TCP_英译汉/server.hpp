#include "socket.hpp"
#include <functional>

typedef std::function<void(std::string& req, std::string* resp)> Handel;

class Tcp_Server{
public:
  Tcp_Server(){
   int ret = socket_.Socket(); 
   if(!ret){
     std::cerr << "create socket failed !" << std::endl;
     exit(1);
   }
  }

  bool Start(uint16_t port, Handel service){
    
    

    if (!socket_.Bind(port)){
      std::cerr << "socket bind failed !" << std::endl;
      exit(1);
    }

    std::cout << "server start ....." << std::endl;

    if(!socket_.Listen(5)){
      std::cerr << "listen error " << std::endl;
    
    }

    while(1){
    //用于接收客户端地址
    std::string c_ip;
    uint16_t c_port;

    int new_sockfd = socket_.Accept(&c_ip, &c_port);
    if(new_sockfd < 0){
      std::cerr << " accept failed" << std::endl;
      continue;
    }      
    while(1){
      std::string buf;
      buf.clear();
      if(!socket_.Recv(new_sockfd, &buf)){
        std::cerr << "recv data failed" << std::endl;
        close(new_sockfd);
        break;
      }

    std::cout << "[" << c_ip << ":" << c_port << "]" << "#" <<" " << buf.c_str() << std::endl;

      std::string resp;
      service(buf,&resp);

      if(!socket_.Send(new_sockfd, resp)){
        
        std::cout << "send failed , connect stop" << std::endl;
      continue;
      }
      
      std::cout << "server echo # " << resp << std::endl; 
      }
     

    }

    std::cout << "server quit !" << std::endl;
    socket_.Close();
}
private:
  Tcp_Socket socket_; 
};
