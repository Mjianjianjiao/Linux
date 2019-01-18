#pragma once 
#include "Head.h"


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

    virtual void ConnectEveneHandel() = 0;
    virtual void ReadEventHandel() = 0;
    virtual void WriteEventHnadel() = 0;

   
    
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

    void Forwording(Channel* clientChannel, Channel* serverChannel);



    struct Connect{
      Channel _clientChannel; //客户端通道
      Channel _serverChannel; //服务器通道
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



