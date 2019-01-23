#include "data.h"
#include <json/json.h>
data::data(){

}
void data::serialize(std::string& out_string){

 // Json::Value root;
  ValueObj root;
  root["_name"] = _name;
  root["_school"] = _school;
  root["_message"] = _message;
  root["_type"] = _type;

  Write w;
//  out_string = w.write(root);
  w.run(root, out_string);
}

void data::unserialize(std::string &in_string){
  
  ValueObj root;
  Read r;
  r.run(in_string, root);
  std::string name = root["_name"];//.asString();
  std::string school = root["_school"];//.asString();
  std::string message = root["_message"];//.asString();
  std::string type = root["_type"];//.asString();
  _name = name;
  _school = school;
  _message = message;
  _type = type;
  
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
