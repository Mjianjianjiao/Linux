#pragma once 
#include "Head.h"
#include "TraceLog.h"
#include <signal.h>

class IgnoreSIGPIPE{
  public:
    IgnoreSIGPIPE(){
      ::signal(SIGPIPE, SIG_IGN);
    }
};

static IgnoreSIGPIPE initPIPE_IGN;

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
      }
    }

    //启动服务开始监听
    void Start();
     
    // 事件循环
    void EventLoop();

    virtual void ConnectEventHandel(int connectfd) = 0;
    virtual void ReadEventHandel(int connectfd) = 0;
    virtual void WriteEventHnadel(int fd);

	void SetNonblocking(int fd)
	{
		
			int flags, s;
			flags = fcntl (fd, F_GETFL, 0);
			if (flags == -1)
				ErrorLog("SetNonblocking:F_GETFL");

			flags |= O_NONBLOCK;
			s = fcntl (fd, F_SETFL, flags);
			if (s == -1)
				ErrorLog("SetNonblocking:F_SETFL");
		
	}

    enum Socks5State{
            
      AUTH,
      ESTABLISHED,
      FORWARDING,
    };

   	void OpEvent(int fd, int events, int op)
	{
		struct epoll_event event;
		event.events = events;
		event.data.fd = fd;
		if(epoll_ctl(_eventfd, op, fd, &event) < 0)
		{
			ErrorLog("epoll_ctl(op:%d, fd:%d)", op, fd);
		}
	}
  
    //通道
    struct Channel{
      
      int _fd; //描述符
      std::string _buffer; //读写缓冲区

     // bool  _flag；
      
        Channel()
        :_fd(-1)
        {}
    };

    void Forwarding(Channel* clientChannel, Channel* serverChannel);
    void SendInLoop(int fd, const char* buf, int len);
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

    std::map<int, Connect*> _connectMap;
};



