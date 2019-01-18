#pragma once 
#include "Common.h"
#include <iostream>

static std::string GetFileName(const std::string& path){
  char ch = '/';

#ifdef _WIN32 
  ch = '\\';
#endif 
  size_t pos = path.rfind(ch);
  if(pos == std::string::npos)
    return path;
  else 
    return path.substr(pos + 1);  
}


inline static void __TraceDebug(const char* filename, int line, const char* function, const char* format, ...){

#ifdef __TRACE__ 

  fprintf(stdout, "[TRACE][%s : %d] %s : ", GetFileName(filename).c_str(), line, function);

  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  fprintf(stdout, "\n");

#endif 
}

inline static void __ErrorLog(const char* filename, int line, const char* function, const char* format, ...){

#ifdef __DEBUG__ 
//调用函数信息
  fprintf(stdout, "[ERROR][%s : %d] %s: ", GetFileName(filename).c_str(), line, function);
  
//用户的trace信息
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);

  fprintf(stdout, " errmsg : %s, errno: %d\n ", sterror(errno), errno);
#endif 
}

#define TraceLog(...)  __TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define ErrorLog(...) __ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);























