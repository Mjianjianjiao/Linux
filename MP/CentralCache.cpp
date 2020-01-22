#include "CentralCache.h"
#include "PageCache.h"

CentralCache CentralCache::_inst;

Span* CentralCache::GetOneSpan(SpanList* spanlist, size_t bytes)
{
	Span* span = spanlist->begin();
	while (span != spanlist->end())
	{
		if (span->_objlist != nullptr)
			return span;

		span = span->_next;
	}

	// ��pagecache����һ���µĺ��ʴ�С��span
	size_t npage = ClassSize::NumMovePage(bytes);
	Span* newspan = PageCache::GetInstance()->NewSpan(npage);

	// ��span���ڴ��и��һ����bytes��С�Ķ��������
	char* start = (char*)(newspan->_pageid << PAGE_SHIFT);
	char* end = start + (newspan->_npage << PAGE_SHIFT);
	char* cur = start;
	char* next = cur + bytes;
	while (next < end)
	{
    
		NEXT_OBJ(cur) = next;
      cur = next;
		next = cur + bytes;
	}
	NEXT_OBJ(cur) = nullptr;
	newspan->_objlist = start;
	newspan->_objsize = bytes;
	newspan->_usecount = 0;

	// ��newspan���뵽spanlist
	spanlist->PushFront(newspan);
	return newspan;
}

size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t num, size_t bytes)
{
	size_t index = ClassSize::Index(bytes);
	SpanList* spanlist = &_spanlist[index];

	Span* span = GetOneSpan(spanlist, bytes);

	void* cur = span->_objlist;
	void* prev = cur;
	size_t fetchnum = 0;
	while (cur != nullptr && fetchnum < num)
	{
		prev = cur;  
		cur = NEXT_OBJ(cur);
		++fetchnum;
	}
	
	start = span->_objlist;
	end = prev;  //���ص�endָ��Ҫָ�����һ������
	NEXT_OBJ(end) = nullptr;

	span->_objlist = cur;
	span->_usecount += fetchnum;
	
	if (span->_objlist == nullptr){
		spanlist->Erase(span);
		spanlist->PushBack(span);
	}


	return fetchnum;
}

void CentralCache::ReleaseListToSpans(void* start, size_t byte_size)
{
	//�ҵ�Ҫ�黹��span������λ��
	int index = ClassSize::Index(byte_size);
	SpanList* spanlist = &_spanlist[index];
//	std::unique_lock<std::mutex> lock(spanlist->_mtx);

	while (start != nullptr){

		void* next = NEXT_OBJ(start);
		//��map ���ҵ����Ӧҳ��span
		Span* span = PageCache::GetInstance()->MapObjectToSpan(start);
		//span-��usecount == 0  ��ʾspan �г�ȥ�Ķ��󶼻�������
		//�ͷ�span �ص�pageCache ���кϲ�

		


		//��ȡ�����Ŀ�һ�����λ��
		NEXT_OBJ(start) = span->_objlist;
		span->_objlist = start;

		
		if (--span->_usecount == 0)
		{
			//����span������cache��ȡ��
			spanlist->Erase(span);

			//��span���л�ԭ��ʼ
			span->_next = nullptr;
			span->_prev = nullptr;
			span->_objsize = 0;
			span->_objlist = nullptr;
			//����pagechache
      

			PageCache::GetInstance()->ReleaseSpanToPageCache(span);
		}

		//���span ��objlist != nullptr  ��һ��span �Ƶ�ͷ�ϣ�������һ�ε�ȡ
		if (span->_objlist != nullptr){
			spanlist->Erase(span);
			spanlist->PushFront(span);
		}
		start = next;
	}
}