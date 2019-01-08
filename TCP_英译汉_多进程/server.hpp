#include "socket.hpp"
#include <functional>
#include <signal.h>
typedef std::function<void(std::string& req, std::string* resp)> Handel;

class Tcp_Server{
public:
  Tcp_Server(){
   int ret = socket_.Socket(); 
   if(!ret){
     std::cerr << "create socket failed !" << std::endl;
     exit(1);
   }

   //在多进程中，不需要父进程的阻塞式的等待，所以忽略掉SIGCHLD信号
   signal(SIGCHLD, SIG_IGN);
  }

  //父进程创建子进程处理连接请求
  bool ProcessConnect(int new_sockfd, std::string c_ip, uint16_t c_port, Handel service){

    int ret = fork();
    if(ret > 0){
      //parents  创建父进程后，父进程负责接收请求，子进程负责处理，为了防止accept返回的描述符过多，造成泄漏，所以关闭
      close(new_sockfd);
    }else if(ret == 0){
      //child 子进程负责处理请求
      
    while(1){
      std::string buf;
      buf.clear();
      if(!socket_.Recv(new_sockfd, &buf)){
        std::cerr << "recv data failed" << std::endl;
        close(new_sockfd);
        break;
      }

    std::cout << "[" << c_ip << ":" << c_port << "]" << "#" <<" " << buf.c_str() << std::endl;

      std::string resp;
      service(buf,&resp);

      if(!socket_.Send(new_sockfd, resp)){
        
        std::cout << "send failed , connect stop" << std::endl;
        continue;
      }
      
      std::cout << "server echo # " << resp << std::endl; 
      }

    }else{
      std::cerr << "fork error" << std::endl;
      return false;
    }

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
     
    ProcessConnect(new_sockfd, c_ip, c_port, service);
    }

    std::cout << "server quit !" << std::endl;
    socket_.Close();
}
private:
  Tcp_Socket socket_; 
};
