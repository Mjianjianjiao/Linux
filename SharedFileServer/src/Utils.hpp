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
#include <vector>
#include <errno.h>
#include <sstream> 
#include <dirent.h>
#include <fcntl.h>
#define LOG(...) do{ fprintf(stdout, __VA_ARGS__); fflush(stdout);}while(0)
#define WWWROOT "www"
#define MAX_HTTPHDR 4096 //所能接收http头部的最大长度
#define MAX_PATH 256
#define MAX_BUFF 4096
std::unordered_map<std::string, std::string> g_err_desc = {
  {"200", "OK"},
  {"400", "Bad Quest"},
  {"403", "Forbidden"},
  {"404", "Not Found"},
  {"405", "Method Not Allowed"},
  {"413", "Request Entity Too Large"}
};

//文件的类型
std::unordered_map<std::string, std::string> g_mime_type = {
  {"txt", "text/plain"},
  {"rtf", "application/rtf"},
  {"gif", "image/gif"},
  {"jpg", "image/jpeg"},
  {"avi", "video/x-msvideo"},
  {"gz",  "application/x-gzip"},
  {"tar", "application/x-tar"},
  {"mpg","video/mpeg"},
  {"au", "audio/basic"},
  {"midi","audio/midi"},
  {"ra", "audio/x-pn-realaudio"},
  {"ram", "audio/x-pn-realaudio"},
  {"midi","audio/x-midi"},
  {"html", "text/html"}
};


class Utils{
  public:
    static int Split(std::string &src, const std::string &seg, std::vector<std::string> &list){
      //将数据进行分割
      int num = 0;
      //key: val\r\nkey: val\r\nkey: val  从key开始先找到\，在找到下一个key
      size_t index = 0;
      size_t pos = 0;
      while(index < src.length()){
        pos = src.find(seg, index);
        if(pos == std::string::npos)
          break;
        list.push_back(src.substr(index, pos - index));
        num++;
        index = pos + seg.length();
      }

      if(index < src.length())
      { 
        list.push_back(src.substr(index,pos - index));
        num++;
      }

      return num; //总共分割的的数据
    }

    static void TimeToGMT(time_t t, std::string &gmt){
      //strftime 将一个结构体中的时间按一定的格式进行转换，返回字符串的长度
      //gmtime 将时间戳准换成格林威治时间， 返回一个结构体  
      //fomat 结构体 %a一周中的第几天  %d一个月中的第几天 ,%b月份的名称 %Y年份
      struct tm *mt = gmtime(&t);
      char tmp[128] = {0};
      int len = strftime(tmp, 127, "%a, %d %Y %H:%M:%S GMT", mt);
      gmt.assign(tmp, len);
    }

    static std::string  GetErrNumState(const std::string &code)
    {
      auto it = g_err_desc.find(code);
      if(it == g_err_desc.end())
      {
        LOG("Unknow error %s\n", strerror(errno));
        return " Unknow Error";
      }   
      return it->second;
    }


    static void DigitTostr(int64_t num, std::string &str){
      std::stringstream ss;
      ss << num;
      str = ss.str();
    }

    static int64_t StrToDigit(std::string &str){
      int64_t num;
      std::stringstream ss;
      ss << str;
      ss >> num;
      return num;
    }

    static void MakeETag(int64_t size, int64_t ino, int64_t mtime, std::string& etag){
      std::stringstream ss;
      ss<< "\"" << std::hex <<ino<<"-" <<std::hex<<size<<"-"<<std::hex<<mtime;
      etag = ss.str();
    }
    
    static void GetMime(std::string &file, std::string &mime){
      size_t pos;
      pos = file.find_last_of(".");
      if(pos == std::string::npos)
      {
        mime = g_mime_type["unknow"];
        return;
      }

     //
      std::string suffix = file.substr(pos + 1);
      auto it = g_mime_type.find(suffix);
      if(it == g_mime_type.end()){
        mime = g_mime_type["unkonw"];
      }else{
        mime = it->second;
      }

    }
};

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

class HttpResponse
{
    //文件请求（完成文件下载、列表功能）接口
    //CGI请求接口
    private:
        int _cli_sock; 
        //Etag: "inode-mtime(最后修改时间)-fsize(wenjain daxiao)"\r\n 
        std::string _etag;//表明文件是否修改过 假如浏览器已经下载过的，就不再返回了，通过以下的属性来判断304
        std::string _mtime; //文件的最后一次修改时间
        std::string _cont_len; //请求的长度
        std::string _date;//系统的响应时间 
        std::string _fsize;
        std::string _mime;
    public:
        HttpResponse(int sock):_cli_sock(sock){}
        bool InitResponse(RequestInfo& req_info){
     
          Utils::TimeToGMT(req_info._st.st_mtime, _mtime);
          Utils::MakeETag(req_info._st.st_ino, req_info._st.st_mtime, req_info._st.st_size, _etag);
          time_t t = time( NULL );
          Utils::TimeToGMT(t, _date);
      //   _etag += req_info._st.st_ino;
      //   _etag += req_info._st.st_mtime;
      //   _etag += req_info._st.st_size;  //文件大小
     
          Utils::DigitTostr(req_info._st.st_size, _fsize);
          _mime = _gesc_
           return true;
        }

        bool ProcessFile(RequestInfo & info) //文件下载功能
        {
         
          LOG("进入下载\n");
          std::string rsp_header;
          rsp_header = info._version + " 206 OK\r\n";
          rsp_header += "Connection: close\r\n";
          rsp_header += "Content-Type: " + _mime + "\r\n";
          rsp_header += "Etag: " + _etag + "\r\n";
          rsp_header += "Lastmodify：" + _mtime + "\r\n";
          rsp_header += "Date: " + _date + "\r\n";
          SendData(rsp_header);

          LOG("rsp_header [ %s ]\n", rsp_header.c_str());


          int fd = open(info._path_phys.c_str(), O_RDONLY);
          if(fd < 0){
            info.SetError("400");
            ErrHandler(info);
            return false;
          }

          int rlen = 0;
          char tmp[MAX_BUFF];
          while((rlen = read(fd, tmp, MAX_BUFF) > 0)){
            tmp[rlen] = '\0';
            //不能用string 或char*来发送数据 ，因为可能会在文件中有0  //可能会造成对端关闭
            SendData(tmp);
          }  
          close(fd);

          //md5sum 对文件进行验证
          

          //断点续传
        // IF- range : etag 
        // Range: bytes=1-100  保存数据请求的范围
       //分块传输使用206  不能用200 ，200返回完整的内容
       //Accept-Ranges: bytes   是否能断点续传
          return true;
        }

        bool ProcessList(RequestInfo &info) //文件列表功能
        {
          LOG("AT ProcessList \n");
          std::string rsp_header;
          rsp_header = info._version + " 200 OK\r\n"; rsp_header += "Connection: close\r\n";
          rsp_header += "Content-Type: text/html\r\n";
          if(info._version == "HTTP/1.1"){
              rsp_header += "Transfer-Ecoding: chunked\r\n";
          }
          
          rsp_header += "Etag: " + _etag + "\r\n";
          rsp_header += "Last - Modify：" + _mtime + "\r\n";
          rsp_header += "Date: " + _date + "\r\n\r\n";
      
          SendData(rsp_header);
          LOG("[%s] \n" , rsp_header.c_str());
           LOG("Send Header \n"); 
          //recv收到0 表示本端关闭链接 
          std::string rsp_body;
          rsp_body = "<html><head>";
          rsp_body += "<meta charset='UTF-8'>"; 
          rsp_body += "<title>共享文件服务器</title>";
          rsp_body +="</head><body><h1> 当前文件路径" + info._path_info + "</h1>";
          rsp_body += "<hr/><ol>";
          //rsp_body += "<input  type >" 
          LOG("[%s] \n" , rsp_body.c_str());
					SendData(rsp_body);
              //获取目录下的每一个文件，组织处html信息，chunke传输
              //scandir 判断目录下有那些文件  返回值为当前文件数 struct 中有不带路径的文件名列表  取出文件名后 与当前目录进行组合，通过stat获取文件信息  第三个为0 表示不过滤  filt(struct *dirent ) {if(dirent->d->_name, "."){
              //return 1;
              //else return 0;
              //过滤掉 . 目录   
              //}}
              //
            
            struct dirent **p_dirent = NULL;
            int  num = scandir(info._path_phys.c_str(), &p_dirent, 0, alphasort);
            LOG("NUM %d\n", num);
            for(int i = 0; i < num ; ++i){

              std::string file_html;
              std::string file = info._path_phys + p_dirent[i]->d_name;  //当前文件的全路径
              LOG("当前请求展示路径： %s\n", file.c_str());//要注意此时生成的请求的路径后面不带/， 需要自行添加 
              struct stat st;
              if(stat(file.c_str(), &st) < 0)
              {
                LOG("stat error \n");
                continue;
              }

              std::string mtime;
              Utils::TimeToGMT(st.st_mtime, mtime);

              std::string mime;
              Utils::GetMime(file, mime);

              std::string filesize;
              Utils::DigitTostr(st.st_size / 1024, filesize); //kb

             	file_html += "<li><strong><a href=";
              file_html += info._path_info; //加粗的链接
              file_html += p_dirent[i]->d_name ;
              file_html += ">"; 
              file_html += p_dirent[i]->d_name ;
              file_html += "/</a></strong>";
              file_html += "<br /><small>";
              file_html += "modified: " + mtime + "<br />";
              file_html += mime + "-" + filesize + " kbytes";
              file_html += "<br /><br /></small></li>";
              SendCData(file_html);
              LOG("%d %s \n", i, file_html.c_str());
        }
          
          rsp_body = "</ol><hr/></body></html>";
          SendCData(rsp_body);
          SendCData("");
          return true;
        }

        bool SendData(std::string str){
          if(send(_cli_sock, str.c_str(), str.length(), 0) < 0){
              return false;
          }

          return true;
        }

        bool SendCData(std::string data){

          //判断协议是否为1.1
            if(data.empty())
            {
              SendData("0\r\n\r\n");
            //  LOG("data ISEmpty\n");
              return true;
            }

            LOG("data %s", data.c_str());
            std::stringstream ss;
            ss << std::hex <<data.size() << "\r\n";
            SendData(ss.str());
            SendData("\r\n");

            SendData(data);
            SendData("\r\n");
            return true;
        }

        bool ProcessCGI(RequestInfo &info) //CGI请求处理,进行程序替换，需要直到程序文件名称
        {
            //创建管道 
            int in[2];//从子进程进行数据的发送
            int out[2]; //从子进程接收数据

            if((pipe(in) < 0) || (pipe(out) < 0)){
              info.SetError("500");
              ErrHandler(info);
              return false;
            }

            pid_t pid;
            pid = fork();
            if(pid < 0){
              info.SetError("500");
              ErrHandler(info);
              return false;
            }else if(pid == 0){
              //设置请求行数据
              setenv("METHOD", info._method.c_str(), 1);
              setenv("PATH_INFO", info._path_info.c_str(), 1);
              setenv("VERSION", info._version.c_str(), 1);
              setenv("QUERY_STRING", info._query_string.c_str(), 1);

              //传递首部字段
              for(auto it = info._hdr_list.begin(); it != info._hdr_list.end(); it++){
                  setenv(it->first.c_str(), it->second.c_str(), 1);
              }
              
              close(in[1]);
              close(out[0]);

              dup2(in[0], 0);
              dup2(out[1], 1);

              execl(info._path_phys.c_str(), info._path_phys.c_str(), NULL);
              exit(0);

            }

           //父进程关闭in[0] 与 out[1]
            
            close(in[0]);
            close(out[1]);


            //父进程向子进程传递正文数据通过out管道，
            char buf[MAX_BUFF];
            auto it = info._hdr_list.find("Content-Length"); //如果没有正文就不传递
            if(it != info._hdr_list.end()){
              int64_t content_len = Utils::StrToDigit(it->second);
              int rlen = recv(_cli_sock, buf, MAX_BUFF, 0);
              if(rlen <= 0)
                return false;

              if(write(in[1], buf, rlen) < 0)
                return false;
            }

            //组织响应头部       
          std::string rsp_header;
          rsp_header = info._version + " 200 OK\r\n"; rsp_header += "Connection: close\r\n";
          rsp_header += "Content-Type: txt/html\r\n";
          if(info._version == "HTTP/1.1"){
              rsp_header += "Transfer-Ecoding: chunked\r\n";
          }
          rsp_header += "Etag: " + _etag + "\r\n";
          rsp_header += "Last - Modify：" + _mtime + "\r\n";
          rsp_header += "Date: " + _date + "\r\n\r\n";
          SendData(rsp_header.c_str());
         
          
          std::string rsp_body;

          rsp_body = "<html><head>";
          rsp_body += "<meta charset='UTF-8'>"; 
          rsp_body += "<title>共享文件服务器</title>";
          rsp_body +="</head><body><h1> 当前路径" + info._path_info + "</h1>";
          //rsp_body += "<input  type >" 
          //按钮
          rsp_body += "<form action='/upload' method='POST' enctype='multipart/form-data'>";
          rsp_body += "<input type='file' name='FileUpload' />";
          rsp_body += "<input type='submit' value='上传' />";
          rsp_body += "</form>";
          rsp_body += "<hr /><ol>";
          
          SendData(rsp_body.c_str());
          while(1){
            char buf[MAX_BUFF] = {0};
            int rlen = read(out[0], buf, MAX_BUFF);
            if(rlen == 0)
              break;
                            
            send(_cli_sock, buf, rlen, 0);
          }

          rsp_body = "<html><body><h1>UPLOAD SUCCESS! </h1></body></html>";
          SendData(rsp_body.c_str());
        }

        //处理错误响应
        bool ErrHandler(RequestInfo& info){
          std::string rsp_header;
          //首行
          //头部 Cotent-Length Date
          //空行
          //正文 rsp_body = "<html><body><h1><404><h1></body></html>"
          rsp_header = info._version + " " + info._err_code + " ";
          rsp_header += Utils::GetErrNumState(info._err_code) + "\r\n";

          time_t t =time(NULL);
          std::string gmt;
          Utils::TimeToGMT(t, gmt);
          rsp_header += "Date: " + gmt + "\r\n";
          std::string rsp_body;
          rsp_body = "<html><body><h1>" + info._err_code;
          rsp_body += "<h1></body></html>";
          std::string cont_len;
          Utils::DigitTostr(rsp_body.length(), cont_len);

          rsp_header += "Content-Length: " + cont_len + "\r\n\r\n";
          
          send(_cli_sock, rsp_header.c_str(), rsp_header.length(), 0);
          send(_cli_sock, rsp_body.c_str(), rsp_body.length(), 0);
        return true;
        }
        
        void CGIHandler(RequestInfo &info){
          InitResponse(info);
          ProcessCGI(info);
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
        
        bool RecvHttpHeader(RequestInfo& info)//接收HTTP请求头
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
            LOG("ret : %d\n", ret);
            LOG("[ %s ]\n", buf);
            char* ptr = strstr(buf, "\r\n\r\n");
            LOG("Ptr %p", ptr);
            if((ptr == NULL) && (ret == MAX_HTTPHDR)){
              _req_info.SetError("413"); //头部太长
              return false;
            }
            else if(ret >= MAX_HTTPHDR){
              usleep(1000);
              continue;
            }


          LOG("recv header3\n");
            int hdr_len = ptr - buf; //读取到的位置减去起始位置，就是头部的长度
            LOG("hdr_len %d\n", hdr_len);
            _http_header.assign(buf, hdr_len);
            recv(_cli_sock, buf, hdr_len + 4, 0);//将\r\n\r\n读走
            LOG("header:[%s\n]",_http_header.c_str());
            break;

          }
          return true;
        }
      
        bool PathIsLegal(RequestInfo &info){
          //判断路径是否合法   stat 输出的文件信息，通过他的返回值判断文件是否存在   Linux 文件名的最大长度为256 是一个宏
          //stat获取状态信息出错，表示无法fangwe到文件
          LOG("判断文件的是否正确\n");
          if(stat(info._path_phys.c_str(), &info._st) < 0){
            LOG("SetError 404\n");
            info.SetError("404");
            return false;
          }
          LOG("文件正确\n");
         
          char tmp[MAX_PATH] = {0};
          realpath(info._path_phys.c_str(), tmp);  
         
          info._path_phys = tmp; 
          LOG("所要查找的路径： %s\n", info._path_phys.c_str());
          if(info._path_phys.find(WWWROOT) == std::string::npos){
            info.SetError("403");
            return false;
          }
         // return true;
        // info.SetError("404");
        // return false;
        return true;
        }

        bool ParseFirstLine(std::vector<std::string> hdr_list, RequestInfo& info)// 解析首行
        {
          std::vector<std::string> line_list;
          if(Utils::Split(hdr_list[0], " ", line_list) != 3){
            info._err_code = "400";
            return false;
          } 

          //打印查看首行
          for(int i = 0; i < 3; i++)
          LOG("FIRSTLINE : %s\n", line_list[i].c_str());  
          
          std::string url;
          info._method = line_list[0];
          url = line_list[1];
          info._version = line_list[2];

          if(info._method != "GET" && info._method != "POST" && info._method != "HEAD"){
            info._err_code = "405"; //请请方法不被允许
            return false;
          }
          
        if(info._version != "HTTP/0.9" && info._version != "HTTP/1.0" && info._version != "HTTP/1.1"){
          info._err_code = "400";
          return false;
        }

        //url : /upload?key=val&key=val
        size_t pos = url.find("?");
        if(pos == std::string::npos){
          info._path_info = url;
        }else{
          info._path_info = url.substr(0, pos);
          info._query_string = url.substr(pos + 1);
        }

        info._path_phys = WWWROOT + info._path_info;

        LOG("要请求的路径：%s\n", info._path_info.c_str());
        

        //realpath 将一个路径转换成据对路径  缺点： 请求路径不存在，会造成段错误,先判断再转换

        return PathIsLegal(info);
        
        }

        bool ParseHttpHeader(RequestInfo& info)//解析http请求头部
        {
          //请求头解析  
          //请求方法 url 协议版本\r\n
          //key: val\r\nkeyval
          std::vector<std::string> hdr_list;
          Utils::Split(_http_header, "\r\n", hdr_list);

          //查看解析好的数据
         // for(int i = 0; i < hdr_list.size(); i++)
          // LOG("%s\n", hdr_list[i].c_str());
        
        if(ParseFirstLine(hdr_list, info) == false)
          return false;

      
        for(size_t i = 1; i < hdr_list.size();  ++i){
          size_t pos = hdr_list[i].find(": ");
          info._hdr_list[hdr_list[i].substr(0, pos)] = hdr_list[i].substr(pos + 2);
        }

        for(auto it = info._hdr_list.begin(); it != info._hdr_list.end(); ++it){
      //    std::cout << "[" << it->first << "]" << ": " << *it << std::cout;
        }

        return true;
        
        }
          
  //      RequestInfo& GetRequestInfo(); //向外提供解析结果
  //      bool ErrHandler(RequestInfo &info);//处理错误响应
  //      
      //      bool CGIHandler(RequestInfo &info){
      //         InitResponse(info); //初始化cgi 响应信息
      //         ProcessCGI(info); //执行cgi响应
      //  }
  
  
   //   bool RequestIsCGI(RequestInfo& info){

    //      return true;
    //   }
        bool FileIsDir(RequestInfo &info)
     {
           std::string path = info._path_info; 
          if(info._st.st_mode & S_IFDIR){
              if(path[path.length() - 1] != '/')
              info._path_info.push_back('/');
              //return true;
              
          //}else{
              std::string phys = info._path_phys;
              if(phys[phys.length() - 1] != '/')
              info._path_phys.push_back('/');
              return true;
          }
          return false;
        }


       bool FileHandler(RequestInfo &info, HttpResponse &rsp){
     
            
           rsp.InitResponse(info); //初化文件响应信息
          
            if(FileIsDir(info)){ //判断文件请求是否是目录
               rsp.ProcessList(info);  //执行展示
             //  LOG();
            }else{
               rsp.ProcessFile(info); //执行文件下载
             }
            return true;
        }
     
       bool RequestIsCGI(RequestInfo &info){
          if(((info._method == "GET") && !info._query_string.empty() )|| info._method == "POST")
            return true;
          return false;
        }      

   
};

