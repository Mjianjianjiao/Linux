#pragma once 
#include "Common.h"
#include "ThreadCache.h"
#include "PageCaceh.h"

void* ConcurrentAlloc(size_t bytes)
{
	if (bytes > MAXBYTES)
	{
		size_t  roundsize = ClassSize::_Roundup(bytes, 1 << PAGE_SHIFT);  //��Ҫ���뵽ҳ���������� ��ҳΪ������
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


//����һ��
//free �ͷ���Ҫָ����С
//���1�� �������ĸ��ֽڣ�������Ŵ�С�� ÿ������ʱ�����ĸ��ֽڣ�������˷�
//��sapn�м�һ��span�д�ſ�Ĵ�С�ĳ�Ա�� ͨ��ָ���ҵ�ҳ��ͨ��map�ҵ�span
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