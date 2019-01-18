#include "Head.h"
#include "Epoll.h"
#include "TraceLog.h"


void EpollServer::Start(){

  //创建套接字
  _listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(_listen_sockfd < 0){
    ErrorLog("create socket");
  }

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_port);
  
  //绑定
  if(bind(_listen_sockfd, (sockaddr*)&addr,  sizeof(addr)) < 0){

    ErrorLog("bind socket");
    return;

  }

  //监听
  if(listen(_listen_sockfd,10000) < 0){
    ErrorLog("listen socket");
    return;
  }
  
  //事件
  _eventfd = epoll_create(MAX_EVENT);
  if(_eventfd < 0){
    ErrorLog("epoll create");
    return;
  }

  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = _listen_sockfd;

  if(epoll_ctl(_eventfd, EPOLL_CTL_ADD, _listen_sockfd, &event) < 0){
    ErrorLog("EPOLL_CTL_ADD listen EPOLLIN");
    return;
  }

  EventLoop();

}

  void EpollServer::EventLoop(){

  struct epoll_event events[MAX_EVENT];
  while(1){

  int size = epoll_wait(_eventfd, events, MAX_EVENT, -1);
  if(size < 0){
    ErrorLog("epoll_wait error");
  }

  for(int i = 0; i < size; ++i){
    if(events[i].data.fd == _listen_sockfd){
      struct sockaddr addr;
      socklen_t* len;
      int connectfd = accept(_listen_sockfd, &addr, &len);
      if(connectfd < 0){
        ErrorLog("accept error");
      }

      TraceLog("client connect");

    }else if(events[i].events & EPOLLIN){

      ReadEventHandel(events[i].data.fd);

    }else if(events[i].events & EPOLLOUT){

      WriteEventHnadel(events[i].data.fd);
    }else {
        ErrorLog("events ");
    }
  }
 }
}
  
  void EpollServer::Forwording(Channel* clientChannel, Channel* serverChannell){

  const size_t bufflen = 4096;
  char buf[bufflen];
  int recv_len = recv(clientChannel->_fd, buf, bufflen);
  if(recv_len > 0){
    int send_len = send(serverChannel->_fd, buf, recv_len);
    
    if(send_len < recv_len){
      SendInLoop();
    }else if(recv_len == 0){
    }
  }

}

