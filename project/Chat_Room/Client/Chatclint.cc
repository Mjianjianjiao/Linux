#include "client.h"
#include <stdlib.h>
void usage(std::string proc){
  std::cout << " usage : " << proc << "ip " << "port" << std::endl;
}

int main(int argc, char* argv[]){
  if(argc != 3){
    usage(argv[0]);
    return 1;
  }

  Client cl(argv[1], atoi(argv[2]));

  std::string message;
  for(;;){
    std::cout << "please Enter#  "<< std::endl;
    std::cin >> message;
    cl.send_data(message);
  }

  return 0;
}
