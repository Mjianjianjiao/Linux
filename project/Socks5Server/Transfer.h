#pragma once 
#include "Head.h"
#include "Epoll.h"

class TransferServer : public EpollServer{
  public:
    TransferServer(const char* socks5ServerIP, int socks5ServerPort, int selfPort = 8000)
      :EpollServer(selfPort){
     //需要连接socks5服务器的IP端口，以及自己启动时需要的端口继承自父类的数据    
        memset(&_socks5addr, 0, sizeof(struct sockaddr_in) );
        _socks5addr.sin_family = AF_INET;
        _socks5addr.sin_port = htons(socks5ServerPort);
        _socks5addr.sin_addr.s_addr = inet_addr(socks5ServerIP);
      }

    virtual void ConnectEveneHandel(int connectfd);
    virtual void ReadEventHandel(int connectfd);
    virtual void WriteEventHnadel(int connectfd);

//子类不写虚函数，会继承父类虚的属性，没有需要清理的无需谐函数
    

  protected: 
    struct sockaddr_in _socks5addr;  //socks5server的地址
};
