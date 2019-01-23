#pragma  once 

#include <iostream>

class data{

  public:
  data();
  ~data();

  void serialize(std::string  &out_string);
  void unserialize(std::string &in_string);
  public:
    std::string _name;
    std::string _school;
    std::string _message;
    std::string _type;
};
