#pragma once
#include "socket.hpp"
#include <assert.h>
class Udp_Client{
  public:
    Udp_Client(string ip, uint16_t port)
      : ip_(ip)
      , port_(port){
      
        assert(sock_.Socket());
    }

    ~Udp_Client(){
      sock_.Close();
    }

    bool RecvFrom(string* buf){

     bool ret =  sock_.RecvFrom(buf, &ip_, &port_);
    
     return ret ;
    }

    bool SendTo(string& buf){
      sock_.SendTo(buf, ip_, port_);
    }

  private:
    Udp_Socket sock_;

    //存放服务器端的ip 与 port
    string ip_;
    uint16_t port_;
};

