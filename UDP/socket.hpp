#pragma once 
#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

typedef  struct sockaddr  sockaddr;
typedef struct sockaddr_in sockaddr_in;


class Udp_Socket{
  public:
    Udp_Socket():fd_(-1){}

    bool Socket(){
        fd_ = socket(AF_INET , SOCK_DGRAM , 0);
        if(fd_ < 0)
        {
          perror("socket error");
          return false;
        }

        return true;
    }

    bool Bind(const std::string& ip_ , uint16_t port_){
      sockaddr_in addr;

      //将所传入的ip与端口号放入地址结构体中，与套接字进行绑定
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip_.c_str());
      addr.sin_port = htons(port_); 
  
      int ret = bind(fd_ , (sockaddr*)&addr , sizeof(addr));
      if(ret < 0)
      {
        cerr << "bind error" << endl;
        return false;
      }

      return true;
    }

    bool RecvFrom(string* buf , string* ip = NULL , uint16_t* port = NULL){

      //使用一个结构体作为输出型参数将发送数据方的端口与ip地址保存下来
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      char tmp[1024];

      ssize_t s = recvfrom(fd_, tmp, sizeof(tmp)-1, 0, (sockaddr*) &addr , &len);
      if(s < 0 )
      {
        cerr << "recvfrom error" << endl;
        return false;
      }

      buf->assign(tmp, s); 
      if(ip != NULL)
          *ip =inet_ntoa(addr.sin_addr); 
      
      if(port != NULL)
        *port = ntohs(addr.sin_port);
    
      return true;
    }


    bool SendTo(string& buf , string& ip , uint16_t port){

      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str()); 
      addr.sin_port = htons(port);
      socklen_t len = sizeof(addr);

      ssize_t wr = sendto(fd_, buf.c_str(), buf.size(), 0, (sockaddr*)&addr, len);
      if(wr < 0){

        cerr << "sendto error" << endl;
        return false;
      }

      return true;
    }
    
    void Close(){
      close(fd_);
    }

  private:
    int fd_;
};
