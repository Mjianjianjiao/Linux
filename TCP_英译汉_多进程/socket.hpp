#pragma once 
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
//将两个地址格式重定义简化书写
typedef struct sockaddr  sockaddr;
typedef struct sockaddr_in sockaddr_in;

class Tcp_Socket{
  public:
    Tcp_Socket():fd_(-1){
    }

    ~Tcp_Socket(){

      close(fd_);
    }

    //创建套接字
    bool Socket(){

       fd_ = socket(AF_INET, SOCK_STREAM, 0); //protocol 参数设置为0 表示选择默认的协议 ，SOCK_STREAM 对应 TCP
       if(fd_ < 0){
         return false;
       }

       return true;
    }

    //绑定套接字
    bool Bind(uint16_t port){
      
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = htons(INADDR_ANY);

      int ret = bind(fd_, (sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        return false;
      }
      return true;
    }

    //建立连接
    bool Connect(std::string& ip , uint16_t port){

      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());

      int ret = connect(fd_, (sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        std::cerr << "connect error" << std::endl;
        return false;
      }
      return true;
    }

    //监听
    bool Listen(int backlog){
      int ret = listen(fd_, backlog);
      if(ret < 0){
        return false;
      }
      return true;
    }

    //接收请求
    int Accept(std::string* ip = NULL, uint16_t* port = NULL){{

      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      int new_fd = accept(fd_, (sockaddr*)&addr, &len);
      if(new_fd < 0){
        std::cerr << "accept error" << std::endl;
        return -1;
      }

      if(ip != NULL){
       *ip = inet_ntoa(addr.sin_addr);
      }
      if(port != NULL){
        *port = ntohs(addr.sin_port);
      }

      return new_fd;
    }

    }
    //接收数据
    bool Recv(int sockfd, std::string* buf){
     
      
      char tmp[1024] = {0};
      ssize_t ret = recv(sockfd, tmp, sizeof(tmp), 0);
      if(ret < 0){
        return false;
      }
      if(ret == 0){
        return false;
      }

      buf->assign(tmp, ret);
      return true;

    }

    //发送数据
    bool Send(int sockfd, std::string& buf){

      int ret = send(sockfd, buf.c_str(), buf.size(), 0);
      if(ret < 0){
        return false;
      }

      return true;
    }

    void Close(){
      close(fd_);
    }

    int GetSockfd(){
      return fd_;
    }

  private:
    int fd_; //套接字描述符
};
