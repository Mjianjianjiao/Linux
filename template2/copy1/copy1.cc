
#include <iostream>
#include <cstring>
#include <typeinfo>
#include <string>
using namespace std;
/*
 *使用函数进行通用拷贝的实现，但时在基本类型的确定时需要循环，仍然造成一些效率问题 
 * 
 * 
 */
template<class T>
bool IsBasicType(const T* Type){

  const char* type_arr[] = { "int" , "char" , "float" , "double" , "long" , "short" , "long long" };
  for(int i = 0 ; i < sizeof(type_arr)/sizeof(type_arr[0]); ++i)
  {

    if(strcmp(type_arr[i] , Type) == 0)
      return true;
    return false;
  }

}


template<class T>
void Copy(T* dest , T* src, size_t size){

  if(IsBasicType(typeid(T).name()))
    memcpy(dest,src,size*sizeof(T));
  else{
  for(int i = 0; i < size; i++)
    {
      dest[i] = src[i];
    }
  }
}

int main()
{
//  string s1[] = {"www" , "eee" , "rrr"};
 // string s2[3];

  float s1[] = {11.1,33.2,33.4};
  float s2[3];
  Copy<float>(s2,s1,3);
  for(int i = 0; i < sizeof(s2)/sizeof(s2[0]); ++i)
     cout << s2[i] << endl; 
  return 0;
}
