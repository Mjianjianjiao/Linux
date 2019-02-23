#include "Utils.hpp"

enum _boundry_type{

  BOUNDRY_NO = 0, //没有boundry
  BOUNDRY_FIRST,
  BOUNDRY_MIDDLE ,
  BOUNDRY_LAST,
  BOUNDRY_BAK  //不完整的boundry  ,如果是不完整的， 就要进行读取判断
};
class Upload{
  //对正文据的解析并践行解析
  private:
    int _file_fd; //遇到文件名就将那个文件描述符打开
    int64_t _cont_len;
    std::string _file_name;
    std::string _f_boundry;
    std::string _m_boundry;
    std::string _l_boundry;

  public:
    Upload():_file_fd(-1){}
    /*流程：
     * InitUploadInfo:初始化文件上传信息
     *  1.从Content-type 获取boundry内容
     *  2.组织first/middl/last boudry 信息
     *  
     * ProcessUpload: 处理文件上传
     *  1.从正文起始位置匹配first boundry
     *  2.从boundry 头信息获取上传的文件名称
     *  3.继续循环从剩下的信息中匹配middle boundry(可能会有多个middle boundry) 
     *    a. middle boundry 之前的数据是上一个文件的数据
     *    b.将之前的数据存储到文件中关闭文件
     *    c.从头信息中获取文件名，若有， 则打开文件
     *  4.当匹配到last boundry时
     *    a.将boundry 之前的数据存储到文件
     *    b.文件上传处理完毕
     *  
     *  
     * */
  private: 
    bool MatchBoundry(char *buf, int blen, int* boundry_pos){
      //不能全文找， first_boundry: ------boundry\r\n
      //middle_boundry  \r\n-------boundry\r\n
      //last_boundry \r\n------boundry--
      //从起始位置匹配first_boundry
      //按内存进行比较，判断boundry的位置
      int ret = memcmp(buf, _f_boundry.c_str() ,_f_boundry.length());
      if(!ret){
        return BOUNDRY_FIRST;
      }

      //只能以内存比较， 不能以字符串比较， 防止其中出现0
      for(int i = 0; i < blen; i++){
        //防止出现半个boundry

        if((blen -i) < _m_boundry.length()){
          if(!memcmp(buf + i, _m_boundry.c_str(), _m_boundry.length())){
            *boundry_pos = i;
            return BOUNDRY_MIDDLE;
          }
          else if(!memcmp(buf + i, _l_boundry.c_str(), _m_boundry.length())){
            *boundry_pos = i;
            return BOUNDRY_LAST;
          }else {
            //否则如果剩余长度小于boundry 的长度，防止出现半个boundry  所以要进行部分匹配
            int cmp_len = (blen -i) > _m_boundry.length()?_m_boundry.length():(blen -i);
            if(!memcmp(buf+i, _m_boundry.c_str(),cmp_len) )
            *boundry_pos = i;
              return BOUNDRY_BAK;
            if(!memcmp(buf+i, _l_boundry.c_str(),cmp_len) )
            *boundry_pos = i;
              return BOUNDRY_BAK;
          }
        }
      }

      return BOUNDRY_NO;
      return true;
    }



    //初始化booundry数据
    bool InitUploadInfo(){

      umask(0);
      char* ptr = getenv("Content-Length");
      if(ptr == NULL)
      {
        fprintf(stderr, "have no Content-Length");
        return false;
      }
      _cont_len = Utils::StrToDigit(ptr);

      ptr = getenv("Content-Length");
      if(ptr == NULL)
      {
        fprintf(stderr, "have no Content-Length");
        return false;
      }
      std::string boundry_sep = "boundary=";
      std::string content_type = ptr;
      size_t pos = content_type.find(boundry_sep);
      if(pos == std::string::npos){
        fprintf(stderr, "content_type have no boundry\n");
        return false;
      }

      std::string boundry = content_type.substr(pos + boundry_sep.length());
      _f_boundry = "--" +boundry ; //"\r\n"
      _m_boundry = "\r\n" + _f_boundry + "\r\n";
      _l_boundry = "\r\n" + _f_boundry + "--";

    }
    //处理文件上传, 将文件数据进行上传
    bool ProcessUpload(){

      int64_t  tlen = 0, blen = 0;
      char buf[MAX_BUFF];
      while(tlen < _cont_len){
        int len = read(0, buf + blen, MAX_BUFF - blen);
        blen += len; //当前buf中的数据

        int boundry_pos, content_pos;
        int flag = MatchBoundry(buf, blen, &boundry_pos);

        if(flag == BOUNDRY_FIRST){
          //1.从boundry 头中获取文件名
          //2若获取文件名称ing，则创建文件打开文件
          //3.将头信息从buf中移除，剩下的数据进一步匹配
          if(GetFileName(buf, &content_pos)){
            CreateFile();
            blen -= content_pos;
            memmove(buf, buf + content_pos, blen);  //用正文将之前的数据覆盖掉
          }else{
            blen -= _f_boundry.length();
            memmove(buf, buf + _f_boundry.length(), blen);
          }
        }

        while(1){

          int flag = MatchBoundry(buf, blen, &boundry_pos);
          if(flag == BOUNDRY_MIDDLE){
            break;
          }

          //匹配middle Boundry成功
          //将boundry 之前的数据写入文件， 将数据从buf中移一处
          //
          WriteFile(buf, boundry_pos - buf);
          CloseFile();
          blen -= boundry_pos;
          memmove(buf, buf + content_pos, blen);
          
          if(GetFileName(buf, &content_pos)){
            CreateFile();
            blen -= content_pos;
            memmove(buf, buf + content_pos, blen);  //用正文将之前的数据覆盖掉
          }else{
            if(content_pos == 0){
             //头信息不全，找不到\r\n\r\n, 跳出
              break;
            }
            blen -= _m_boundry.length();
            memmove(buf, buf + _m_boundry.length(), blen);
          }
        }


        flag = MatchBoundry(buf, blen, &boundry_pos);
        if(flag == BOUNDRY_LAST){
            WriteFile(buf, boundry_pos - buf)
            CloseFie();
          return true;
        }

        flag = MatchBoundry(buf, blen, &boundry_pos);
        if(flag == BOUNDRY_MIDDLE){
        }

        flag = MatchBoundry(buf, blen, &boundry_pos);
        if(flag == BOUNDRY_BAK){
          //1.将类似boundry位置之前的数据写入文件
          //移除之前的数据
          //剩下的数据不动，重新接收数据，补全后匹配
          WriteFile(buf, boundry_pos - buf);
          blen -= boundry_pos;

        }
        flag = MatchBoundry(buf, blen, &boundry_pos);
        if(flag == BOUNDRY_NO){
          //直接将buf中的所有写入文件
          WriteFile(buf, len);
          blen = 0;
        }

        tlen +=len;
      }

     return false;

    }



    bool CreateFile(){
      _file_fd = open(_file_name.c_str(), O_CREAT | O_WRONLYi, 0664);
      if(_file_fd < 0){
        fprintf(stderr, "open err %s \n", strerror(errno));
        return false;
      }
      return true;
    }

    bool CloseFile(){
      if(_file_fd != -1){
        close(_file_fd);
        _file_fd = -1;
      }

      return true;
    }

    bool GetFileName(char* buf, int *content_pos){
      char* ptr = NULL;
      ptr = strstr(buf, "\r\n\r\n");
      if(ptr == NULL){
        *content_pos = 0;
        return false;
      }
// md5sum      
//certutil -hashfike  file 
      *content_pos = ptr - buf;
      *content_pos += 4;
      std::string header;
      header.assign(buf, ptr - buf);

      std::string file_sep = "filename=\"";
      size_t pos = header.find(file_sep);
      if(pos == std::string::npos)
        return false;

      _file_name = header.substr(pos + file_sep.length());
      pos = _file_name.find("\"");
      if(pos == std::string::npos)
        return false;

      _file_name.erase(pos); //
       _file_file = WWWROOT;
      _file_name += "/" + file;
      fprintf(stderr, "upload file:[%s] \n", _file_name.c_str());
      return true;
    }

    bool WriteFile(char *buf, int len){
      if(_file_fd != -1)
        write(_file_fd, buf, len);

      return true;
    }
};










int main(){

  Upload upload;

  std::string rsp_body;
  if(upload.InitUploadInfo() == false){
    return false;
  }

  if(upload.ProcessUpload() == false){
    rsp_body = "";
  }else{
    rsp_body ="";
  }
  std::cout << rsp_body << endl;
  fflush(stdout);
}
//缓冲区的数据没有读完， 就会造成链接重置
