#include "client.h"
#include <assert.h>
Client::Client(std::string server_ip, int server_port ){
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(server_ip.c_str());
  server.sin_port = htons(server_port);
  
}

void Client::Init_Client(){

  _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(_sockfd < 0){
    std::cerr << " socket error" << std::endl; 
    return;
  }

}


void Client::recv_data(std::string &out_string){

  char buf[SIZE];
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  ssize_t s = recvfrom(_sockfd, buf, SIZE, 0, (struct sockaddr*)&peer, &len);
  if(s > 0){
    buf[s] = 0 ;
    out_string = buf;
  }else if(s == 0){
    std::cerr << "server close" << std::endl;
  }else if(s == -1){
    std::cerr << "recv error" << std::endl;
  }
}

void Client::send_data(const std::string &in_string){

  sendto(_sockfd, in_string.c_str(), in_string.size(), 0, (struct sockaddr*)&server, sizeof(server)); 

}



Client::~Client(){
  close(_sockfd);
}
