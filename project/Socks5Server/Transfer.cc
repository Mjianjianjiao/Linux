#include "Head.h"
#include "Transfer.h"
#include "TraceLog.h"

void TransferServer::ConnectEventHandel(int connectfd){

  //连接到socks服务器
  int socks5fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socks5fd < 0){
    ErrorLog("socket");
    return;
  }

  if(connect(socks5fd, (struct sockaddr*)&_socks5addr, sizeof(_socks5addr)) < 0){
      ErrorLog("connect socks5 server");
      return;
      }


  SetNonblocking(connectfd);
  OpEvent(connectfd, EPOLLIN, EPOLL_CTL_ADD);

  SetNonblocking(socks5fd);
  OpEvent(socks5fd, EPOLLIN, EPOLL_CTL_ADD);

  //创建服务器 与 客户端的连接通道
  Connect* connect = new Connect;
  connect->_state = FORWARDING;
  //将通道信息保存下来
  connect->_clientChannel._fd= connectfd;
  connect->_ref++;

  connect->_serverChannel._fd = socks5fd;
  connect->_ref++;

  _connectMap[connectfd] = connect;
  _connectMap[socks5fd] = connect;
  //


}


  

void TransferServer::ReadEventHandel(int fd){

  std::map<int, Connect*>::iterator it = _connectMap.find(fd);
  if(it != _connectMap.end()){
   //先识别是从哪一边到哪一边的， 
    Connect* connect = it->second;
    Channel* clientChannel = &(connect->_clientChannel); 
    Channel* serverChannel = &(connect->_serverChannel);

    if(fd == connect->_serverChannel._fd){
   
      //完成复用
      std::swap(clientChannel, serverChannel);
    }

    
    //zhun fa 
    Forwarding(clientChannel, serverChannel);
  }
  else {
    assert(false);
  }
}

int main(){
  
  TransferServer server("127.0.0.1",8001, 8000);
  server.Start();

}
