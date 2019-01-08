#include "server.hpp"
#include <unordered_map>

void Usermanual(){
  std::cout << "please input : ./server port " << std::endl;
}
            
std::unordered_map<std::string ,std::string > dict;

void Insert_word(){

  dict.insert(std::make_pair("first", "第一"));
  dict.insert(std::make_pair("second", "第二"));
  dict.insert(std::make_pair("hello", "你好"));
  dict.insert(std::make_pair("world", "世界"));
  dict.insert(std::make_pair("tcp", "脱层皮"));
}
void translate(const std::string& req, std::string* resp){

  auto it = dict.find(req);
  if(it == dict.end()){
    *resp = "未查到";
    return ;
  }


  *resp = it->second; 

}
int main(int argc, char* argv[]){

  if(argc != 2){
    Usermanual();  
    exit(1);
  }


  Tcp_Server server;
  Insert_word();
  server.Start(atoi(argv[1]),translate);
  return 0;
}
