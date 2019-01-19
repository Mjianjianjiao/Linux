#include "Head.h"
#include "Transfer.h"
#include "TraceLog.h"

void TransferServer::ConnectEveneHandel(int connectfd){

  //连接到socks服务器
  int socks5fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socks5fd < 0){
    ErrorLog("socket");
    return;
  }

  if(connect(socks5fd, (const struct sockaddr*)&_socks5addr, sizeof(_socks5addr)) < 0){
      ErrorLog("connect socks5 server");
      return;
      }

  //创建服务器 与 客户端的连接通道
  Connect* connect = new Connect;
  //将通道信息保存下来
  connect->_clientChannel._fd= connectfd;
  connect->_clientChannel._event = EPOLLIN;

  OpEvent(connectfd, connect->_clientChannel._event, EPOLL_CTL_ADD);
  
  connect->_serverChannel._fd = socks5fd;

  _connectMap[connectfd] = connect;
  _connectMap[socks5fd] = connect;
  //


}

void TransferServer::WriteEventHnadel(int connectfd){

  
}

void TransferServer::ReadEventHandel(int fd){

  std::map<int, Connect*>::iterator it = _connectMap.find(fd);
  if(it != _connectMap.end()){
   //先识别是从哪一边到哪一边的， 
    Connect* connect = it->second;
    Channel* clientChannel = &(connect->_clientChannel); 
    Channel* serverChannel = &(connect->_serverChannel);

    if(fd == serverChannel->_fd){
   
      //完成复用
      std::swap(clientChannel, serverChannel);
    }

    
    //zhun fa 
    Forwarding(clientChannel, serverChannel);
  }
}

int main(){
  
  TransferServer server("");
  server.Strat("", 8000, 8001);

}
