#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

void usage(char* proc){
  cout << "usage: " << proc << " port " << endl;
}

int main(int argc, char* argv[]){
  if(argc != 2){
    usage(argv[1]);
    return 1;
  }


  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd < 0){
    cerr << " socket error " << endl; 
    return -1;
  }

  int val = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  struct sockaddr_in addr;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_family = AF_INET;
  if(bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    cerr << " bind error " << endl;
    return -1;
  }

  if(listen(listenfd, 5) < 0){
    cerr << "listen error" << endl;
    return -1;
  }

  int epollfd = epoll_create(1024);
  if(epollfd < 0){
    cerr << "epoll create error" << endl;
    return -1;
  }

  struct epoll_event events[1024], ev;
  ev.data.fd = listenfd;
  ev.events = EPOLLIN;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

  while(1){
    int nfds = epoll_wait(epollfd, events, 1000, 3000);
    if(nfds < 0){
      cerr << "epoll wait error " << endl;
      continue;
    }else if(nfds == 0){
      cout << "timeout " << endl;
      continue;
    }

    struct sockaddr_in cli_addr;
    socklen_t len;
    for(int i = 0; i < nfds; i++){
      if(events[i].data.fd == listenfd){
        int cli_fd = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
        if(cli_fd < 0){
          cerr << "accept error" << endl;
          continue;
        }

        ev.events = EPOLLIN;
        ev.data.fd = cli_fd;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, cli_fd, &ev);
      }
      else if(events[i].events & EPOLLIN){
        //不是监听描述符，且时一个可读事件
        char buf[1024];
        int recv_len = recv(events[i].data.fd, buf, 1024, 0);
        if(recv_len < 0){
          cerr << "recv data error" << endl;
        //  close(suc_events.data.fd);
          continue;
        }

        if(recv_len == 0){
          cerr << "client close" << endl;
          close(events[i].data.fd);
          continue;
        }

        string ip = inet_ntoa(cli_addr.sin_addr);
        int port = htons(cli_addr.sin_port);
        printf("[%s : %d] : %s \n", ip.c_str(), port, buf);
        string resp = "hello";
        send(events[i].data.fd, resp.c_str(), resp.size(), 0);
      }
    }
  }

}
