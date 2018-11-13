#include "string.h"

void myString::String::Reserve(size_t n)
{
  if(n>_capacity)
  {
    char* tmp=new char[n+1];
    strcpy(tmp,_str);
    delete[] _str;
    _str=tmp;
    _capacity=n;
    _str[n+1]=0;
  }
}

void myString::String::Resize(size_t n,char s)
{
    if(n<_size)
    {
       _str[n]=0;  
    }else if(n>_size&&n<_capacity||n>_capacity)
    {
      if(n>_capacity)
        this->Reserve(n);
      int pos=_size;
        while(pos!=n)
        {
          _str[pos++]=s;
        }
        _str[pos]='\0';
    }
}
void myString::String::Push_Back(char ch){
    
      if(_size==_capacity)
       this->Reserve(_capacity*2);
      _str[_size++]=ch;
      _str[_size]='\0';
}
void myString::String::Append(const char* str){
      int len=strlen(str);
      if(_size+len>_capacity)
       this->Reserve((_size+len)*2);
         strcpy(_str+_size,str);
}
myString::String& myString::String::operator+= (char ch){
      
     this->Push_Back(ch);
     return *this;
}
myString::String& myString::String::operator+= (const char* str){
      this->Append(str);
      return *this;
}
size_t myString::String::Find(char ch,size_t pos){
    
      if(pos>_size||pos<1)
        return npos;
      while(pos!=_size)
      {
          if(_str[pos]==ch)
            return pos;
          else pos++;
      }
      return npos;
}
size_t myString::String::Find(char* str ,size_t pos){
      
        if(pos>_size||pos<1)
          return npos;
        int len=strlen(str);

        int i=pos,j=0;
        while(i!=_size)
        {
           int start=i;
          while(j!=len&&_str[i]==str[j])
          {
                j++;
                i++;
          }
          if(j==len)
            break;
          i=start+1;
          j=0;          
        }

        if(j=len&&i!=_size)
          return i-len;
      
        return npos;
}
void myString::String::Insert(size_t pos,char ch){
   if(pos>=_size||pos<1)
     perror("pos error");
   if(_size==_capacity)
     this->Reserve(_capacity*2);
    int cur=_size;
   while(cur!=pos-1)
   {
      _str[cur+1]=_str[cur];
      cur--;
   }
   _size+=1;
   _str[pos]=ch;
}
void myString::String::Insert(size_t pos,char* str){

     if(pos>=_size||pos<0)                                                                                                                                                                                               perror("pos error"); 
   int len=strlen(str);
     if(_size+strlen(str)>_capacity)
       this->Reserve((_size+len)*2);
     _size+=len;
     int cur = _size ;
     while(cur!=pos-1)
     {
       _str[cur+len] = _str[cur];
       cur--;
     }
     while(len--)
     {
        _str[pos++] = *str++;
     }
}
size_t  myString::String::Erase(size_t pos, size_t len)
{
    if(pos>_size || pos<0)
      perror("pos error");
   if(pos+len >= _size)
     return npos;
   int k = len;
   while(k--)
   {
     _str[pos] = _str[pos+len];
     pos++;
   }
   _str[pos]='\0';
   _size-=len;
}
myString::String& myString::String::assign(char* str)
{
   strcpy(_str,str);
   _size=strlen(str);
} 

int main() {
  myString::String s1("hello world");
//  cout<<s1.myString::String::size()<<endl;
//  cout<<s1.myString::String::C_str()<<endl;
    myString::String s2(s1);
//    cout<<s2.myString::String::C_str()<<endl;
  //  myString::String s3;
 //   s3=s2;
 //   cout<<s2.myString::String::C_str()<<endl;
//    s2.myString::String::Reserve(20);
    
 //   cout<<s2.myString::String::Capacity()<<endl;
 //   s2.Resize(5);
    
    //cout<<s2.myString::String::C_str()<<endl;
 //  s2.Resize(35,0);
  //  s2.Push_Back('!');
  //  cout<<s2.myString::String::C_str()<<endl;
  //  s2.Append("!!!!");
  //  cout<<s2.myString::String::C_str()<<endl;
  //  s2+='!';
  //  cout<<s2.myString::String::C_str()<<endl;
  //  s2+="###";
  //  cout<<s2.myString::String::C_str()<<endl;
//  cout<<s2.Find('w',2)<<endl;
 // cout<<s2.Find("wor",2)<<endl;
  //  s2.Insert(6,'h');
 //   s2.Insert(6,"hello");
 //   cout<<s2.myString::String::C_str()<<endl;
  //  s2.Erase(6,5);
 //  cout<<s2.myString::String::C_str()<<endl;
  //   s2.assign("i am auto");
 //   cout<<s2.myString::String::C_str()<<endl;
 //  s2=s1+s1; 
  // cout<<s2.myString::String::C_str()<<endl;
  cout<<s2;
}
