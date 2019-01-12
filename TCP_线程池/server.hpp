#include "socket.hpp"
#include <functional>
#include <pthread.h> 
#include "thread_pool.hpp"
typedef std::function<void(std::string& req, std::string* resp)> Handel;

const int PTH_NUM = 5;


  struct Pthread_arg{
    //创建一个结构体作为参数传入线程的处理函数
    Tcp_Socket *socket_;
    int new_sockfd;
    Handel service;
    std::string ip;
    uint16_t port;
  };




class Tcp_Server{
public:
  Tcp_Server(){
   int ret = socket_.Socket(); 
   if(!ret){
     std::cerr << "create socket failed !" << std::endl;
     exit(1);
   }

   
  }


  void ServerInit(){
     
      pool_ = new PthreadPool(5);
      pool_->PthreadPoolInit();
 
  }
  
  static void ThreadRun(void* arg){

    Pthread_arg* p_arg = (Pthread_arg*)arg;

    while(1){
      std::string buf;
      buf.clear();
      if(!p_arg->socket_->Recv(p_arg->new_sockfd, &buf)){
        std::cerr << "recv data failed" << std::endl;
        close(p_arg->new_sockfd);
        delete p_arg;
        break;
      }

    std::cout << "[" << p_arg->ip << ":" << p_arg->port << "]" << "#" <<" " << buf.c_str() << std::endl;


      std::string resp;
      p_arg->service(buf,&resp);

      if(!p_arg->socket_->Send(p_arg->new_sockfd, resp)){
        
        std::cout << "send failed , connect stop" << std::endl;
        continue;
      }
      
      std::cout << "server echo # " << resp << std::endl; 

  }

}
  

  bool PthreadProcessConnect(int new_sockfd, std::string c_ip, uint16_t c_port, Handel service){ 

    Pthread_arg* arg = new Pthread_arg; //此处只可用new 动态创建对象，静态会导致传参后参数乱码 arg->service = service; arg->socket_ = &socket_;
    arg->new_sockfd = new_sockfd;
    arg->ip = c_ip;
    arg->port = c_port;
    arg->service = service;
    arg->socket_ = &socket_;

 
    Task t(arg, ThreadRun);
    pool_->AddTask(t);
   
    return true;
  }

  bool Start(uint16_t port, Handel service){
    
    
    if (!socket_.Bind(port)){
      std::cerr << "socket bind failed !" << std::endl;
      exit(1);
    }

    std::cout << "server start ....." << std::endl;

    if(!socket_.Listen(5)){
      std::cerr << "listen error " << std::endl;
    
    }

    while(1){
    //用于接收客户端地址
    std::string c_ip;
    uint16_t c_port;

    int new_sockfd = socket_.Accept(&c_ip, &c_port);
    if(new_sockfd < 0){
      std::cerr << " accept failed" << std::endl;
      continue;
    }      
     
    PthreadProcessConnect(new_sockfd, c_ip, c_port, service);
    
    }

    std::cout << "server quit !" << std::endl;
    socket_.Close();
}

~Tcp_Server(){
  socket_.Close();
  delete pool_;
}
private:
  Tcp_Socket socket_; 
  PthreadPool* pool_;
};
