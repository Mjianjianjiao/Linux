#pragma once

#include "Common.h"

class NewMalloc
{
public:
	//������Դ
	static void* Allocate(size_t size);
	//�ͷ���Դ
	static void Deallocate(void* ptr);

private:
  
};
