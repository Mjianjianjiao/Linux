#include "socket.hpp"
#include <assert.h>
#include <functional>

//该服务器所设计的功能是将客户端所发送的消息再发送个客户端

//响应请求的处理函数
//typedef void(*Handel)(string& req ,string& resq);
typedef function< void(string& req, string* resq) > Handel;

class Udp_Server{

  public:
    Udp_Server(){
      assert(sock_.Socket()); //创建套接字，并检查是否创建成功
    }

    //服务器开始运行
    bool Strat(const string&  ip, uint16_t port, Handel handel){

      //绑定套接字
      if(!sock_.Bind(ip,port)){
        return false;
      }
  
      cout << "server start ....." << endl;
      while(1){
    
        //使用输出型参数，存放客户端的IP 与 端口号
        string c_ip;
        uint16_t c_port = 0;
        string buf;
        string res;

        //接收消息
        bool ret = sock_.RecvFrom(&buf, &c_ip, &c_port);
        if(!ret){

          cerr << "server receive error" << endl;
          //如果出错，跳过重新接收
          continue;

        }

        handel(buf,&res); //服务器处理函数
        //对客户端进行响应
        sock_.SendTo(res, c_ip, c_port);  

        printf("[%s:%d] : %s \n" , c_ip.c_str(), c_port, buf.c_str()); 

      }

      sock_.Close();

      return true;
    }

    ~Udp_Server(){
      sock_.Close();
      cout << "server quit " << endl;
    }
  private:
    Udp_Socket sock_;
};
