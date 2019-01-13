#include "socket.hpp"
#include "cal_protocol.hpp"

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

  bool RecvData(reponse* resp){
     int  ret =  recv(socket_.GetSockfd(), resp, sizeof(*resp), 0);
     if(ret <= 0)
       return false;

     return true;
    }
  
  bool SendData(request* req){
      int ret = send(socket_.GetSockfd(), req, sizeof(*req), 0);
      if(ret < 0 )
        return false;
      return true;
    }
    
  void Close(){
      close(socket_.GetSockfd());
    }

private:
  Tcp_Socket socket_;
  std::string ip_;
  uint16_t port_;
};
