#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "des.h"

class FileEncryption
{
public:
    FileEncryption(uint32_t key)
      :des(key)
  {}
    int encrypt(string input);
    int decrypt(string input);
    int cipher (string input,  bool mode);

private:
    DES des;
};

#endif // FILEENCRYPTION_H
