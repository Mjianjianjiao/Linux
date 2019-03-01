#pragma once 
#include "Common.h"
#include "ThreadCache.h"
#include "PageCaceh.h"

void* ConcurrentAlloc(size_t bytes)
{
	if (bytes > MAXBYTES)
	{
		size_t  roundsize = ClassSize::_Roundup(bytes, 1 << PAGE_SHIFT);  //从要对齐到页的整数倍， 以页为对齐数
		size_t  npage = roundsize >> PAGE_SHIFT;

		Span* span = PageCache::GetInstance()->NewSpan(npage);
		void* ptr = (void*)(span->_pageid << PAGE_SHIFT);
		return ptr;
		
	}
	else
	{
		if (tls_threadcache == nullptr)
			tls_threadcache = new ThreadCache;

		return tls_threadcache->Allocate(bytes);
	}
}


//问题一：
//free 释放需要指定大小
//解决1： 多申请四个字节，用来存放大小， 每次申请时都有四个字节，会造成浪费
//在sapn中加一个span中存放块的大小的成员， 通过指针找到页再通过map找到span
void ConcurrentFree(void* ptr)
{
	Span* span = PageCache::GetInstance()->MapObjectToSpan(ptr);
	size_t size = span->_objsize;
	if (size > MAXBYTES)
	{
		PageCache::GetInstance()->ReleaseSpanToPageCahce(span);
	}
	else
	{
		tls_threadcache->Deallocate(ptr, size);
	}
}