#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include "ThreadPool.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Utils.hpp"


const int MAX_LISTEN = 10;
const int MAX_THREAD = 20;


class HttpServer
{
    //为建立的一个tcp服务端程序，接收新连接
    //为新连接组织一个线程池任务，添加到线程池中
    private:
        int _serv_sock;
        ThreadPool *_tp;
    private:
        //http任务处理函数
        static bool HttpHandler(int sock){
//          HttpRequest req(sock);
//          HttpResponse rsp(sock);
//          RequestInfo info;
//
//          if(req.RecvHttpHeader(info) == false){
//            goto out;
//          }//接收请求出错，需要保存错误信息
//
//          if(req.ParseHttpHeader(info) == false){
//            goto out;
//          }
//
//          //判读请求否是cgi请求，
//          if(info.RequestIsCGI()){
//            rsp.CGIHandler(info); // 如是cgi 请求，执行Cgi响应
//          }
//          else{
//            //不是，执行目录列表/文件下载响应
//            rsp.FileHandler(info);
//          }
//
//          out: 
//            rsp.ErrHandler(info);
//            close(sock);
//
          return true;
        }
    public:
				HttpServer()
          :_serv_sock(-1)
           , _tp(NULL) 
        {}

        bool HttpServerInit(std::string ip, uint16_t port) //完成TCP服务端初始化，线程池的初始化
				{
					_serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if(_serv_sock < 0){
						LOG("sock error : %s\n", strerror(errno));
						return false;
					}

					struct sockaddr_in  addr;
					addr.sin_family = AF_INET;
					addr.sin_port = htons(port);
				  addr.sin_addr.s_addr = inet_addr(ip.c_str());
          
					if(bind(_serv_sock, (sockaddr*)&addr, sizeof(addr)) < 0){
						LOG("bind error %s\n",strerror(errno));
						close(_serv_sock);
						return false;
					}

					if(listen(_serv_sock, MAX_LISTEN) < 0){
						LOG("liste error %s\n", strerror(errno));
						close(_serv_sock);
						return false;
					}

					_tp = new ThreadPool(MAX_THREAD);
					if(_tp->ThreadPoolInit() == false){
						LOG("thread pool init error \n");
					}
					return true;
				}
					
        bool Start()//开始获取服务器的新连接--创建任务，任务入队
        {
          while(1){

            sockaddr_in cli_addr;
            socklen_t len = sizeof(cli_addr);
            int sock = accept(_serv_sock, (sockaddr*)&cli_addr, &len);
            if(sock < 0 ){
              LOG("accept error %s\n", strerror(errno));
              continue;
            }

            HttpTask ht;
            ht.SetHttpTask(sock, HttpHandler);
            _tp->PushTask(ht);
          }
        }
};

void UserTip(char* str){
  std::cout << "please input " << str << " ip port !!" << std::endl;
}

int main(int argc, char* argv[]){


  if(argc < 2)
  UserTip(argv[0]);

  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  HttpServer server;
  server.HttpServerInit(ip, port);
  server.Start();

}
