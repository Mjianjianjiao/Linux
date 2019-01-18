#include "Head.h"
#include "Transfer.h"

void TransferServer::ConnectEveneHandel(int connectfd){

  //连接到socks服务器
  int socks5fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socks5fd == -1){
    ErrorLog("socket");
    return;
  }

  if(connect(socks5fd, (const struct sockaddr*)&_socks5addr, sizeof(_socks5addr)) < 0){
      ErrorLog("connect socks5 server");
      return;
      }

  //创建服务器 与 客户端的来连接通道
  Connect* connect = new Connect;
  connect->_clientChannel.fd = fd;
  connect->_clientChannel._event = EPOLLIN;

  OpEvent(fd, connect->_clientChannel._event, );
  
  connect->serverChannel._fd = socks5fd;

  _connectMap[fd] = connect;
  _connectMap[socks5fd] = connect;


I
}

void TransferServer::WriteEventHnadel(int connectfd){

  
}

void TransferServer::ReadEventHandel(int connectfd){

  map<int, Connect*>::iterator it = _connectMap.find(fd);
  if(it != _connect){
    
    Connect* connect = it->second;
    Channel* clientChannel = connect->_clientChannel; 
    Channel* serverChannel = connect->_serverChannel;

    if(fd == serverChannel->_fd){
   
      //完成复用
      swap(clientChannel, serverChannel);
    }

    Forording(client);
  }
}

int main(){
  
  TransferServer server("");
  server.Strat();

}
