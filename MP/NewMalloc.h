#pragma once

#include "Common.h"

class NewMalloc
{
public:
	//申请资源
	static void* Allocate(size_t size);
	//释放资源
	static void Deallocate(void* ptr);

private:
  
};
