#pragma once 

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>

#define SIZE 1024
class Client{

  public:
    Client(std::string server_ip, int server_port);
    void Init_Client();
    void recv_data(std::string &out_string);
    void send_data(const std::string &in_string);
    ~Client();
  private:
    int _sockfd;
    struct sockaddr_in server;
};
