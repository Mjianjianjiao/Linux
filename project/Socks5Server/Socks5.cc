#include "Socks5.h"
#include <netdb.h>
void Socks5Server::ConnectEventHandel (int connectfd){

  TraceLog("new connect event : %d", connectfd);

  SetNonblocking(connectfd);
  OpEvent(connectfd, EPOLLIN, EPOLL_CTL_ADD);

  Connect* con = new Connect;
  con->_state = AUTH;
  con->_clientChannel._fd = connectfd;
  _connectMap[connectfd] = con;
  con->_ref++;
}


//0 表示数据的失败
//1 success
//-1 faild
int Socks5Server::AuthEventHandel(int fd){

  int buf[260];
  int rlen = recv(fd, buf, 260,MSG_PEEK); //探测 并不把数据读走
  if(rlen < 0){
    return -1;
  }
  else if(rlen < 3){
    return -2;
  }
  else {
    recv(fd, buf, rlen, 0);
    
    if(buf[0] != 0x05){
      ErrorLog("not socks5");
      return -1;
    }

    return 1;
  }
}

//失败返回-1
//数据没到返回-2
//成功返回serverfd
int Socks5Server::EstablishedEventHandel(int fd){

  char buf[256];
  int rlen = recv(fd, buf, 256, MSG_PEEK);
  if(rlen < 10){
    return -2;
  }
  else if(rlen == -1){
    return -1;
  }
  else if(rlen == 1){
    
    char ip[4];
    char port[2];

    recv(fd, buf, 4, 0);
    
    char addr_type = buf[3];
    
    if(addr_type == 0x01){
      //ipv4
      recv(fd, ip, 4, 0);
      recv(fd, port, 2, 0);
    }
    else if(addr_type == 0x03){
      //domin_name
      char len = 0;
      recv(fd, &len, 1, 0);
      recv(fd, buf, len, 0);

      //recv port 
      recv(fd, port, 2, 0);
      TraceLog("DOMAINnAME: %s", buf);
      buf[len] = '\0';
      
      //通过域名获取ip
      struct hostent* htpr = gethostbyname(buf);
      if(htpr == NULL){
        TraceLog("gethostbyname(): %s", buf);
        return -1;
      }

      memcpy(ip, htpr->h_addr, htpr->h_length);

    }
    else if(addr_type == 0x04){
      ErrorLog("dont support ipv6");
    }
    else{
      ErrorLog("invalid address type");
      return -1;
    } 

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr.s_addr, ip, 4);
    addr.sin_port = *((uint16_t*)port);

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
      ErrorLog("server socket");
      return -1;
    }

    if(connect(serverfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
      ErrorLog("connect faild");
      close(serverfd);
      return -1;
    }

    return serverfd;
  }
}

void Socks5Server::RemoveConnect(int fd){
  
  OpEvent(fd, EPOLLIN, EPOLL_CTL_ADD); 
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



void Socks5Server::Forwarding(Channel* clientChannel, Channel* serverChannel){
  
  char buf[4096];
  int rlen = recv(clientChannel->_fd, buf, 4096, 0);
  if(rlen < 0){
    ErrorLog("recv : %d :" , clientChannel->_fd );
  }
  else if(rlen == 0){
    //client channel 
    shutdown(serverChannel->_fd, SHUT_WR);
    RemoveConnect(clientChannel->_fd);  //不一定是真的关闭
    
  }
  else{   
    
    if(send(serverChannel->_fd, buf, rlen, 0) < rlen){
      TraceLog("recv : %d -> send: %d", rlen);
    }
  } 
}




void Socks5Server::ReadEventHandel(int connectfd){
  
  TraceLog("read event:%d", connectfd);
  std::map<int, Connect*>::iterator it = _connectMap.find(connectfd); 
  if(it != _connectMap.end()){

   Connect* con = it->second;

  if(con->_state == AUTH){
    
    char rep[2];
    rep[0] = 0x05;
    
    int ret = AuthEventHandel(connectfd);
    if(ret == 0){
      return;      
    }
    else if(ret == 1){
      rep[1] = 0x00;
      con->_state = ESTABLISHED;
    }
    else if(ret == -1){
      rep[1] = 0xFF;
      RemoveConnect(connectfd);
    }

    if(send(connectfd, rep, 2, 0) != 2){
      ErrorLog("auth reply");
    }

  }else if(con->_state == ESTABLISHED){

    //回复
    char reply[10] = {0};
    reply[0] = 0x05;

    int serverfd = EstablishedEventHandel(connectfd);
    if(serverfd == -1){
      
      reply[1] = 0x01;
      ErrorLog("EstablishedEventHandel failed");
      RemoveConnect(connectfd);
      return;

    }else if(serverfd == -2){
      return;
    }else{
      reply[1] = 0x00;
      reply[3] = 0x01;
    }

    if(send(connectfd, reply, 10, 0) != 10){
      ErrorLog("establich connect failed");
    } 
    
    SetNonblocking(serverfd);
    OpEvent(serverfd, EPOLLIN, EPOLL_CTL_ADD);
    con->_state = FORWARDING;
    _connectMap[serverfd] = con;
    con->_state = FORWARDING;
    con->_ref++;

  }else if(con->_state == FORWARDING){
    
    Channel* clientChannel = &con->_clientChannel;
    Channel* serverChannel = &con->_serverChannel;

    if(connectfd == serverChannel->_fd){
      std::swap(clientChannel, serverChannel);
    }

    //client->server 
    Forwarding(clientChannel, serverChannel);

  }
  else{
    assert(false);
  }

  }else {
    assert(false);
  }

}


int main(){

  Socks5Server server(8001);

  server.Start();
}
