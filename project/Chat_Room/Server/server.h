#pragma once 
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

#define SIZE 1024
class Server{

  public:
    Server(int port);
    void Init_Server();
    void recv_data(std::string &out_string);
    void send_data(const std::string &in_string, const struct sockaddr_in &peer, const socklen_t &len);
    void broadcast();
    ~Server();
  private:
    int _sockfd;
    int _port;
};
