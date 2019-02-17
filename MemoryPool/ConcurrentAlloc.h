#pragma once 
#include "Common.h"
#include "ThreadCache.h"


void* ConcurrentAlloc(size_t bytes)
{
	if (bytes > MAXBYTES)
		return malloc(bytes);
	else
	{
		if (tls_threadcache == nullptr)
			tls_threadcache = new ThreadCache;

		return tls_threadcache->Allocate(bytes);
	}
}

void ConcurrentFree(void* ptr, size_t bytes)
{
	if (bytes > MAXBYTES)
		free(ptr);
	else
	{
		tls_threadcache->Deallocate(ptr, bytes);
	}
}