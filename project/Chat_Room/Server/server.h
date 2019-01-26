#pragma once 
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include "data_pool.h"
#include <map>
#include <stdlib.h>
#define SIZE 1024

class Server{

  public:
    Server(int port);
    void Init_Server();
    void recv_data(std::string &out_string);
    void send_data(const std::string &in_string, const struct sockaddr_in &peer);
    void broadcast();
    ~Server();
  private:
    int _sockfd;
    int _port;
    data_pool pool;
    std::map<uint32_t, struct sockaddr_in> online;
};
