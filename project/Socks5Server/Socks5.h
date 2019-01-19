#pragma once 
#include "Epoll.h"
#include "Head.h"
#include "TraceLog.h"

class Socks5Server : public EpollServer{

  public:
    Socks5Server(int port)
      :EpollServer(port){}

    int AuthEventHandel(int fd);
    int EstablishedEventHandel(int fd);
    void RemoveConnect(int fd);
    void Forwarding(Channel* clientChannel, Channel* serverChannel);

    virtual void ConnectEventHandel(int connected);
    virtual void ReadEventHandel(int connected);
  //  virtual void WriteEventHandel(int connected);

  protected:

};
