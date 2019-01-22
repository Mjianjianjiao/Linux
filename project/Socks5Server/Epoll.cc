#include "Head.h"
#include "Epoll.h"
#include "TraceLog.h"


void EpollServer::Start(){

  //创建套接字
  _listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(_listen_sockfd < 0){
    ErrorLog("create socket");
  }
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_port);
 

  //端口复用
  int val = 1;
  setsockopt(_listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  //绑定
  if(bind(_listen_sockfd, (const struct sockaddr*)&addr,  sizeof(addr)) < 0){

    ErrorLog("bind socket");
    return;

  }

  //监听
  if(listen(_listen_sockfd,10000) < 0){
    ErrorLog("listen socket");
    return;
  }
  TraceLog("epoll server listen on %d" ,_port);
  //事件
  _eventfd = epoll_create(100000);
  if(_eventfd < 0){
    ErrorLog("epoll create");
    return;
  }

  SetNonblocking(_listen_sockfd);
  OpEvent(_listen_sockfd, EPOLLIN, EPOLL_CTL_ADD);
  EventLoop();
}

  void EpollServer::EventLoop(){

  struct epoll_event events[100000];
  while(1){

  int size = epoll_wait(_eventfd, events, 100000, 0);
  if(size < 0){
    ErrorLog("epoll_wait error");
  }

  for(int i = 0; i < size; ++i){
    if(events[i].data.fd == _listen_sockfd){
      //表示此时有连接来了，使用accept进行接收
      struct sockaddr addr;
      socklen_t len;
      int connectfd = accept(_listen_sockfd, &addr, &len);
      if(connectfd < 0){
        ErrorLog("accept error");
      }

      TraceLog("client connect");

      ConnectEventHandel(connectfd);

    }else if(events[i].events & EPOLLIN){
 //  
      ReadEventHandel(events[i].data.fd);

    }else if(events[i].events & EPOLLOUT){
  //
      WriteEventHnadel(events[i].data.fd);
    }else {
        ErrorLog("events %d ", events[i].data.fd);
    }
  }
 }
}

 void EpollServer::RemoveConnect(int fd){
  
  OpEvent(fd, EPOLLIN, EPOLL_CTL_DEL); 
  std::map<int, Connect*>::iterator it = _connectMap.find(fd);
  if(it != _connectMap.end()){
    Connect* con = it->second; 
    if(--con->_ref == 0){
      delete con;
      _connectMap.erase(it);
    }
  }else{
    assert(false);
  }
}



void EpollServer::SendInLoop(int fd, const char* buf, int len){
  
  int slen = send(fd, buf, len, 0);
  if(slen < 0){
    ErrorLog("send to %d ", fd);
  }
  else if(slen < len){
    TraceLog("recv %d bytes, send %d bytes, left %d send in loop", len , slen , len - slen);
    std::map<int, Connect*>::iterator it = _connectMap.find(fd);
    if(it != _connectMap.end()){
      Connect* con = it->second;
      Channel* channel = &con->_clientChannel;
      if(fd == con->_serverChannel._fd){

        channel = &con->_serverChannel;
      }

      int events = EPOLLOUT | EPOLLIN | EPOLLONESHOT;
      OpEvent(fd, events, EPOLL_CTL_MOD);

      channel->_buffer.append(buf+slen);
    }
    else 
      assert(false);
  }
}



 void EpollServer::Forwarding(Channel* clientChannel, Channel* serverChannel){

  const size_t bufflen = 4096;
  char buf[bufflen];
  
  int recv_len = recv(clientChannel->_fd, buf, bufflen, 0);
  if(recv_len > 0){
    
      buf[recv_len] = '\0';
      SendInLoop(serverChannel->_fd, buf, recv_len); //使事件循环将未发送完的数据进行发送
    }
    else if(recv_len == 0){
      //client channel 
      shutdown(serverChannel->_fd, SHUT_WR);
      RemoveConnect(clientChannel->_fd);
    }
    else{
      TraceLog("recv : %d", clientChannel->_fd);
    }
  }




void EpollServer::WriteEventHnadel(int fd){
  std::map<int, Connect*>::iterator it = _connectMap.find(fd);
  if(it != _connectMap.end()){

    Connect* con = it->second;
    Channel* channel = &con->_clientChannel;
    if(fd == con->_serverChannel._fd){
      channel = &con->_serverChannel;
    } 

    std::string buf;
    buf.swap(channel->_buffer);
    SendInLoop(fd, buf.c_str(), buf.size()) ;
  }
  else{
    assert(fd);
  }
}


