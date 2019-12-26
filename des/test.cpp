#include "des.h"
#include "des.cpp"
#include "des3.h"
#include "des3.cpp"
#include "fileencryption.cpp"
#include <stdio.h>
#include <sys/time.h>
void test(){
  
  ui64 input = 	0x8774B8E8C73BCA7D;
  ui64 key   = 0x2BD6459F82C5B300;
  printf("Key: %016lX\n", key);
 
  DES des(key);
  uint64_t result = des.encrypt(input);
  printf("E: %016lX\n", result);

  result = des.decrypt(result);
  printf("D: %016lX\n", result);
  printf("P: %016lX\n", input);
}



void test2(){

  
  ui64 input = 	0x8774B8E8C73BCA7D;
  ui64 key1   = 0x2474B8E8C73BCA7D;
  ui64 key2   = 0x2BD6859F8DC5B3FF;
  ui64 key3   = 0x2BD6459F82C5B300;
  

  cout<<"加密内容: "<<hex<<input<<endl<<endl;;

  ui64 result;
  DES des(key1);
  DES3 des3(key1,key2,key3);
  struct timeval tstart, tend;
  cout << "使用Des进行加密："<<endl;
  gettimeofday(&tstart,NULL);
  result = des.encrypt(input);
  gettimeofday(&tend,NULL);
  long int  time = 1000000*(tend.tv_sec - tstart.tv_sec);
  cout << "加密用时："<<time<<endl;
  cout<< "加密结果："<<hex<<result<<endl;
  gettimeofday(&tstart,NULL);
  result = des.decrypt(result);
  gettimeofday(&tend,NULL);
   time = 1000000*(tend.tv_sec - tstart.tv_sec);
  cout << "解密用时："<<time<<endl;
  cout<< "解密结果："<<hex<<result<<endl;
  cout<<"---------------------------------------"<<endl;

  cout << "使用3Des进行加密："<<endl;
 // start = clock();
  gettimeofday(&tstart,NULL);
  result = des3.encrypt(input);
//  end = clock();
//  time = (end - start);
  gettimeofday(&tend,NULL);
   time = 1000000*(tend.tv_sec - tstart.tv_sec);
  cout << "加密用时："<<time<<endl;
  cout<< "加密结果："<<hex<<result<<endl;
 // start = clock();
  gettimeofday(&tstart,NULL);
  result = des3.decrypt(result);
//  end = clock();
  gettimeofday(&tend,NULL);
 // time =  (end - start);
   time = 1000000*(tend.tv_sec - tstart.tv_sec);
  cout<<"解密用时："<<time<<endl;
  cout<<"解密内容 ："<<hex<<result<<endl;
}



int main(int argc, char* argv[]){
 
//  test2();
  string input;

  if(argc > 3) input = argv[3];

  uint64_t key = strtoll(argv[2],nullptr,16); 

  FileEncryption file(key);

  string choice = argv[1]; 

  if(choice == "-e"){
  cout<< "加密" << endl;
  file.encrypt(input);
  }

  if(choice  == "-d"){
  cout<< "解密" << endl;
  file.decrypt(input);
}

}
