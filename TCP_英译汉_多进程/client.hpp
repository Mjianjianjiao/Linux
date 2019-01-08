#include "socket.hpp"

class Tcp_Client{
public:
  Tcp_Client(std::string ip, uint16_t port):ip_(ip), port_(port){
    if(!socket_.Socket()){
      std::cerr << "create scket failed" << std::endl;
      exit(1);
    }
  }
   
  void ConnectServer(){
    if(!socket_.Connect(ip_, port_)){
      std::cerr << "connect server failed" << std::endl;
      exit(1);
    }
  }
    bool RecvData(std::string* buf){
       return  socket_.Recv(socket_.GetSockfd(), buf);
        
    }
    bool SendData(std::string& buf){
      return socket_.Send(socket_.GetSockfd(), buf);
    }
    
    void Close(){
      close(socket_.GetSockfd());
    }

private:
  Tcp_Socket socket_;
  std::string ip_;
  uint16_t port_;
};
