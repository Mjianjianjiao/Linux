#pragma once 
#include "Head.h"
#include "Epoll.h"

class TransferServer : public EpollServer{
  public:
    TransferServer(const char* socks5ServerIP, int socks5ServerPort, int selfPort = 8000)
      :EpollServer(selfPort){
         
        memset(&_socks5addr, 0, sizeof(struct sockaddr_in) );
        _socks5addr.sin_family = AF_INET;
        _socks5addr.sin_port = htons(socks5ServerPort);
        _socks5addr.sin_addr.s_addr = inet_addr(socks5ServerIP);
      }

    virtual void ConnectEveneHandel(int connectfd);
    virtual void ReadEventHandel(int connectfd);
    virtual void WriteEventHnadel(int connectfd);


    

  protected: 
    struct sockaddr_in _socks5addr;
};
