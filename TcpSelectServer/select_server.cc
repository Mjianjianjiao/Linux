#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void userage(std::string proc){
  std::cout << "userage : " << proc << " port " << std::endl;
}
int main(int argc, char* argv[]){
  if(argc != 2){
    userage(argv[0]);
    return 1;
  }

  int listenfd, clientfd;
  listenfd = socket(AF_INET,SOCK_STREAM, 0);
  if(listenfd < 0){
    std::cerr << " socket error" << std::endl;
    return -1;
  }

  struct sockaddr_in ser_addr;
  ser_addr.sin_family = AF_INET;
  ser_addr.sin_addr.s_addr = INADDR_ANY;
  ser_addr.sin_port = htons(atoi(argv[1]));

  int val = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val) );
  
  int ret = bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
  if(ret < 0){
    std::cerr << "bind error" << std::endl;
    return -1;
  }

  if(listen(listenfd, 5) < 0){
    std::cerr << " listen error " << std::endl;
    return -1;
  }

  int fd_list[1024];
  for(int i = 0; i < 1024; i++){
    fd_list[i] = -1;
  }
  fd_list[0] = listenfd;
  int max_fd = listenfd;
  fd_set readfds;
  while(1){

    //重新将其他描述符放入监听，readfds 每次就绪后会将其他没就绪的置0
    for(int i = 0; i < 1024; ++i){
      if(fd_list[i] != -1){
        FD_SET(fd_list[i], &readfds);
      }
    }

    //选出最大的描述符，select时使用
    for(int i = 0; i < 1024; ++i){
      if(fd_list[i] > max_fd){
        max_fd = fd_list[i];
      }
    }

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    int sel = select(max_fd + 1, &readfds, NULL, NULL, &tv);
    if(sel < 0){   //在select 时客户端断开
      std::cout << "select error" << std::endl;
      FD_ZERO(&readfds);
      continue;
    }else if(sel == 0){
      std::cout << "timeout" << std::endl;
      continue;
    }

    for(int i = 0; i < 1024; i++){
      if(fd_list[i] < 0)
        continue;
         
      struct sockaddr_in cli_addr;
      socklen_t len;
       
      //有事件发生,由于不知到是哪一个描述符，readfds中一旦有就绪的描述符，所以其他的描述符就被置0了；
      if(FD_ISSET(fd_list[i], &readfds)){
        if(fd_list[i] == listenfd){
          //如果是监听描述符
          clientfd = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
          if(clientfd < 0){
            std::cerr << "accept error" << std::endl;
            continue;
           }

          for(int i = 0; i < 1024; ++i){
            if(fd_list[i] == -1){
              fd_list[i] = clientfd;
              break;
            }
          }
               
        }else{
          //不是监听描述符，是与客户端通信的socket
          char buf[1024];      
          int ret = recv(fd_list[i], buf, 1024, 0);
          if(ret <= 0){
            std::cerr << "recv error" << std::endl;
            close(fd_list[i]);
            fd_list[i] = -1;
            continue;
          }
          buf[ret] = 0;
          std::string ip = inet_ntoa(cli_addr.sin_addr);
          uint16_t  port = ntohs(cli_addr.sin_port);
          printf("[%s : %d]: %s\n", ip.c_str(), port, buf);

          if(send(fd_list[i], "what???", 8, 0) < 0){
            std::cerr << "send error" << std::endl;
            continue;
          }

          
        }
      }
    }

  }

}
