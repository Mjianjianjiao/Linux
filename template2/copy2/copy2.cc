#include <iostream>
#include <string>
#include <cstring>
using namespace std;
/*使用类型萃取
 *使用模板的特化，将所有的模板进行特化，使其能够自动识别，从而少了之前
 * 循环判断类型
 */

//标识基本类型
struct BasicType{
  
  static bool Get(){
    return true;
  }

};

//标识自定义类型
struct DefineType{
  static bool Get(){
    return false;
  }
};

template<class T>
struct Type_recog{
  typedef DefineType IsBasicType;
};

template<>
struct Type_recog<int>{
  typedef BasicType IsBasicType;
};


template<>
struct Type_recog<char>{
  typedef BasicType IsBasicType;
};

template<>
struct Type_recog<short>{
  typedef BasicType IsBasicType;
};

template<>
struct Type_recog<float>{
  typedef BasicType IsBasicType;
};

template<>
struct Type_recog<double>{
  typedef BasicType IsBasicType;
};

template<>
struct Type_recog<long>{
  typedef BasicType IsBasicType;
};

template<>
struct Type_recog<long long>{
  typedef BasicType IsBasicType;
};

template<class T>
void Copy(T* dest, T* src ,size_t size){
  if(Type_recog<T>::IsBasicType::Get())
    memcpy(dest,src,size*sizeof(T));
  else {
    for(int i = 0; i < size; i++)
      dest[i] = src[i];
  }
}


int main(){
//  string s1[] = {"www","eee","rrr"};
//  string s2[3];

  int s1[] = {1,2,3};
  int s2[3];
  Copy<int>(s2,s1,3);
  for(int i = 0; i < sizeof(s2)/sizeof(s2[0]); ++i)
    cout << s2[i] << endl;
  return 0;
}
