#pragma once

#include "ConcurrentAlloc.h"

class ThreadCache
{
public:
	//申请资源
	void* Allocate(size_t size);
	//释放资源
	void* Deallocate(void* ptr, size_t size);

	// 从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);

	//对象太长及进行回收
	void ListTooLong(FreeList* freelist, size_t bytes);
private:
	//内存块大小的哈希映射表
	FreeList _freelist[NLISTS];
	//int _tid;
	//ThreadCache* _next;
};

//为每一给线程创建一个各自的全局变量
static _declspec(thread) ThreadCache* tls_threadcache = nullptr;