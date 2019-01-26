#include "server.h"
#include "data.h"

Server::Server(int port)
  : _port(port)
{}

void Server::Init_Server()
{
  _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (_sockfd < 0)
  {
    std::cerr << "socket error!" << std::endl;
    return;
  }
  
  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(_port);
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(_sockfd, (struct sockaddr*)&local, sizeof(local)) < 0)
  {
    std::cerr << "bind error!" << std::endl;
    return;
  }
}

void Server::recv_data(std::string& out_string)
{
  char buf[SIZE];
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  ssize_t s =recvfrom(_sockfd, buf, SIZE, 0, (struct sockaddr*)&peer, &len);
  if (s > 0)
  {
    buf[s] = 0;
    out_string = buf;
    pool.put_message(out_string);
    data d;
    d.unserialize(out_string);

    //根据key值删除，该ip地址（客户端）
    if (d._type == "quit")
    {
      std::map<uint32_t,struct sockaddr_in>::iterator iter = online.find(peer.sin_addr.s_addr);
      if(iter != online.end())
      {
        online.erase(iter->first);
      }

    }
    else 
    {
      online.insert(std::pair<uint32_t, struct sockaddr_in>(peer.sin_addr.s_addr, peer));
    }
  }
  else 
  {
  }
}

void Server::send_data(const std::string& in_string,\
    const struct sockaddr_in &peer)
{
  sendto(_sockfd, in_string.c_str(), in_string.size(), 0,\
      (struct sockaddr*)&peer, sizeof(peer));
}

void Server::broadcast()//广播
{
  std::string message;
  pool.get_message(message);
  
  std::map<uint32_t, struct sockaddr_in>::iterator iter = online.begin();
  for (; iter != online.end(); iter++)
  {
    send_data(message, iter->second);
  }
      
}

Server::~Server()
{
  close(_sockfd);
  _port = -1;
}
