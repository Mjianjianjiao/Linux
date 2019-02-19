#pragma once 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <errno.h>

#define LOG(...) do{ fprintf(stdout, __VA_ARGS__); fflush(stdout);}while(0)

#define MAX_HTTPHDR 4096 //所能接收http头部的最大长度

class RequestInfo{
    //包含HttpRequest解析出的请求信息
    public:
        std::string _method; //请求方法
        std::string _version; //协议版本
        std::string _path_info; //资源路径
        std::string _path_phys; //资源实际路径
        std::string _query_string; //查询字符串
        std::unordered_map<std::string, std::string> _hdr_list; //头部信息中的键值映射
        struct stat _st;//存放文件信息
    public:
        std::string _err_code; //存放错误状态
    public:
        void SetError(const std::string str)
        {
           _err_code = str;
        }
};


class HttpRequest
{
    //http数据的接收接口
    //http数据的解析接口
    //对外提供能够获取处理结果的接口
    private:
        int _cli_sock; 
        std::string _http_header; //头部信息
        RequestInfo _req_info; // 解析出来的结果
    public:
        HttpRequest(int sock):_cli_sock(sock) {}
        
        bool RecvHttpHeader()//接收HTTP请求头
        {
          char buf[MAX_HTTPHDR]; //限定头部的数据长度
          while(1){
            int ret = recv(_cli_sock, buf, MAX_HTTPHDR, MSG_PEEK);
            if(ret <= 0){
              if(errno = EINTR || errno == EAGAIN){
                //EINTER 表示被信号打断
                //ENGAIN 非阻塞时，缓冲区中没有错误
                _req_info.SetError("500"); //服务器被中断，出错
                return false;
                }
              }
            
            char* ptr = strstr(buf, "\r\n\r\n");
            if((ptr == NULL) && (ret == MAX_HTTPHDR)){
              _req_info.SetError("413"); //头部太长
              return false;
            }
            else if(ret < MAX_HTTPHDR){
              usleep(1000);
              continue;
            }

            int hdr_len = ptr - buf; //读取到的位置减去起始位置，就是头部的长度
            _http_header.assign(buf, hdr_len);
            recv(_cli_sock, buf, hdr_len + 4, 0);//将\r\n\r\n读走
            LOG("header:[%s\n]",_http_header.c_str());
            break;

          }
        }
  //      bool ParseHttpHeader();//解析http请求头部
  //      RequestInfo& GetRequestInfo(); //向外提供解析结果
  //      bool ErrHandler(RequestInfo &info);//处理错误响应
  //      
  //      bool CGIHandler(RequestInfo &info){
  //          InitResponse(info); //初始化cgi 响应信息
  //          ProcessCGI(info); //执行cgi响应
  //      }
  //      
  //      bool FileHandler(RequestInfo &info){
  //          InitResponse(info); //初始化文件响应信息
  //          if(DIR){ //判断文件请求是否是目录
  //              ProcessList(info);  //执行展示
  //          }else{ 
  //              ProcessFile(info); //执行文件下载
  //          }
  //       }
};

class HttpResponse
{
    //文件请求（完成文件下载、列表功能）接口
    //CGI请求接口
    private:
        int _cli_sock; 
        std::string _etag;//表明文件是否修改过
        std::string _mtime; //文件的最后一次修改时间
        std::string _cont_len; 
        
    public:
        bool InitResponse(RequestInfo req_info);
        bool ProcessFile(std::string &file); //文件下载功能
        bool ProcessList(std::string &path); //文件列表功能
        bool ProcessCGI(std::string &file); //CGI请求处理,进行程序替换，需要直到程序文件名称
        
};
