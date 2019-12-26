#include "des.h"
#include "des.cpp"
#include "fileencryption.cpp"
#include <stdio.h>
void test(){
  
  ui64 input = 	0x74B8E8C73BCA7D;
  ui64 key   = 0x2BD6459F82C5B300;
  printf("Key: %016lX\n", key);
 
  DES des(key);
  uint64_t result = des.encrypt(input);
  printf("E: %016lX\n", result);

  result = des.decrypt(result);
  printf("D: %016lX\n", result);
  printf("P: %016lX\n", input);
}

int main(int argc, char* argv[]){
 

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
