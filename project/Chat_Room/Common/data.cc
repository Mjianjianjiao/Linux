#include "data.h"
#include <json/json.h>
data::data(){

}
void data::serialize(std::string& out_string){

 // Json::Value root;
  ValueObj root;
  root["name"] = _name;
  root["school"] = _school;
  root["message"] = _message;
  root["type"] = _type;

  Write w;
//  out_string = w.write(root);
  w.run(root, out_string);
}

void data::unserialize(std::string &in_string){
  
  ValueObj root;
  Read r;

 r.run(in_string,root);                                                             
  std::string nick_name1 = root["name"];                                       
  std::string school1 = root["school"];                                             
  std::string message1 = root["message"];        
  std::string  type1 = root["type"];
  _name = nick_name1;
  _school = school1;
  _message=message1;
  _type = type1;
  
}


data::~data(){
}

//int main(){
//  data d;
//  d._name = "最美的夕阳";
//  d._school = "临潼养老院";
//  d._message = "hello";
//  d._type = "null";
//
//  std::string out_string;
//  d.serialize(out_string);
//  std::cout << out_string<<std::endl;
//}
