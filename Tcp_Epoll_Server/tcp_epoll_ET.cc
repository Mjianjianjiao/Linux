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
#include <fcntl.h>
#include <errno.h>
using namespace std;


void SetNoBlock(int fd){
  //先获取文件属性
  int flag = fcntl(fd, F_GETFL, 0);
  //在保留原有属性的基础上进行设置
  fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int recv_data(int fd, char* buf, int len){
  int total_len = 0;
  for(;;){
    int recv_len = recv(fd, buf + total_len, len - total_len, 0);
    if(recv_len <= 0){
      //EINTR表示接收数据时被打断，但缓冲区数据未读完， EAGAIN 表示缓冲区中的数据还未准备好
    if(errno == EINTR || errno == EAGAIN){
        continue;
        
    if(recv_len == -1)
        return -1;
    else 
        return 0;
    
    }

  }

    total_len += recv_len;
    if(recv_len < len - total_len)
      break;

  }

  buf[total_len] = '\0';
  return total_len;
}

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

  SetNoBlock(listenfd);

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
  ev.events = EPOLLIN| EPOLLET;
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

        SetNoBlock(cli_fd);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = cli_fd;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, cli_fd, &ev);
      }
      else if(events[i].events & EPOLLIN){
        //不是监听描述符，且时一个可读事件
        char buf[1024];
      // int recv_len = recv(events[i].data.fd, buf, 1024, 0);
        int recv_len = recv_data(events[i].data.fd, buf, 1024);
        if(recv_len < 0){
          cerr << "recv data error" << endl;
         // close(events[i].data.fd);
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
