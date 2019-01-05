#include "server.hpp"
#include <unordered_map>

unordered_map<string, string> dict;

void handel(string& buf, string* res){

   auto it =  dict.find(buf);
   if(it == dict.end()){
     *res = nullptr;
   }
   else
   *res = it->second;
}

void Insert(){

  dict.insert(make_pair("first","第一"));
  dict.insert(make_pair("second","第二"));
  dict.insert(make_pair("third","第三"));
  dict.insert(make_pair("hello","你好"));
  dict.insert(make_pair("world","世界"));

}

void Usermanual(){
  cout << " use the server : ./server  port " << endl;
}

int main(int argc, char* argv[]){

  if(argc != 2){
    Usermanual();
    exit(1);
  }

  Insert();
  Udp_Server server;

  server.Strat( atoi(argv[1]), handel);
  return 0;
}
