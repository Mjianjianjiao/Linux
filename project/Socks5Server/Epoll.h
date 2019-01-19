#pragma once 
#include "Head.h"
#include "TraceLog.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;


const size_t MAX_EVENT = 100000;

class EpollServer{
  public:
    EpollServer(int port)
    :_port(port)
    ,_listen_sockfd(-1)
    ,_eventfd(-1){

    }

    virtual ~EpollServer(){
      if(_listen_sockfd == -1){
        
        close(_listen_sockfd);
        //event_sockfd_
      }
    }

    //启动服务开始监听
    void Start();
     
    // 事件循环
    void EventLoop();

    virtual void ConnectEventHandel(int connectfd) = 0;
    virtual void ReadEventHandel(int connectfd) = 0;
    virtual void WriteEventHnadel(int fd);

  
    
    void SetNonblocking(int fd){
      int flags = fcntl(fd, F_GETFL, 0);
      if(flags == -1)
        ErrorLog("SetNoblocking: F_GETFL");

      flags |= O_NONBLOCK;
      int s = fcntl(fd, F_SETFL, flags);
      if(s == -1)
        ErrorLog("SetNonblocking: F_SETFL");
    }
    enum Socks5State{
            
      AUTH,
      ESTABLISHED,
      FORWARDING
    };

    void OpEvent(int fd, int events, int how){
      struct epoll_event event;
      event.events = events;
      event.data.fd = fd;

      if(epoll_ctl(_eventfd, how, fd, &event) < 0){
        ErrorLog("epoll_ctl(how : %d , fd : %d )", how, fd);
      }
    }
    
    //通道
    struct Channel{
      
      int _fd; //描述符
      int _event;
      std::string _buffer; //读写缓冲区

     // bool  _flag；
      
        Channel()
        :_fd(-1)
        ,_event(0){
        }
    };

    void Forwarding(Channel* clientChannel, Channel* serverChannel);

    void RemoveConnect(int fd);
        

    struct Connect{
      Socks5State _state;
      Channel _clientChannel; //客户端通道
      Channel _serverChannel; //服务器通道
      int _ref;

      Connect()
        :_state(AUTH)
         ,_ref(0)
      {}
    };

  private:
    //防拷贝
    EpollServer(const EpollServer&);
    EpollServer& operator=(const EpollServer&);

  protected:  //定义成受保护，可以被继承
    
    int _port; // 端kou
    int _listen_sockfd; //监听套接
    int _eventfd; //事件描述符
    static const size_t MAX_EVENT; //事件数组的大小

    std::map<int, Connect*> _connectMap;
};



