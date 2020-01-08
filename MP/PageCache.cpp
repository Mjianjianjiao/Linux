#define _CRT_SECURE_NO_WARNINGS 1

#include"PageCache.h"

PageCache PageCache::_inst;

Span* PageCache::NewSpan(size_t npage)
{
 // std::unique_lock<std::mutex> lock(_mtx);
	if (npage >= NPAGES)
	{
		void* ptr = SystemAlloc(npage);
		Span* span = new Span();
		span->_pageid = (PageID)ptr >> PAGE_SHIFT;
		span->_npage = npage;
		span->_objsize = npage << PAGE_SHIFT;
		_id_span_map[span->_pageid] = span;

		return span;
	}

	//��ϵͳ�������64KС��128ҳ���ڴ��ʱ����Ҫ��span��objsize����һ������Ϊ���ͷŵ�ʱ����кϲ�
	Span* span = _NewSpan(npage);
	//������Ƕ���һ����PageCache����span��ʱ������¼�������span����Ҫ�ָ��ʱ��
	span->_objsize = span->_npage << PAGE_SHIFT;

	return span;
}
Span* PageCache::_NewSpan(size_t npage)
{
	//unique_lock<mutex> lock(_mtx);//����

	if (!_pagelist[npage].Empty())
	{
		return _pagelist[npage].PopFront();
	}

	//_pagelist[npage].Empty(),����������ң�ֱ�����û�У�����ϵͳ����һ��128page��С�Ŀռ䣬Ȼ�󻮷�ΪС�ռ�
	for (size_t i = npage + 1; i < NPAGES; i++)
	{
		SpanList* pagelist = &_pagelist[i];
		if (!pagelist->Empty())
		{
			Span* span = pagelist->PopFront();//�Ƚ���span��pagelist�ó�����Ȼ����зָ���������ľ������ã�ʣ�µĲ����Ӧҳ���������±��λ��
			Span* wantspan = new Span;
			wantspan->_pageid = span->_pageid + span->_npage - npage;
			wantspan->_npage = npage;
			span->_npage -= npage;
			

			for (size_t i = 0; i < wantspan->_npage; ++i)
			{
				_id_span_map[wantspan->_pageid + i] = wantspan;
			}
		    
			_pagelist[span->_npage].PushFront(span);

			return wantspan;
		}
	}
	//��ϵͳ����ռ�
	//void* ptr = VirtualAlloc(NULL, (NPAGES - 1) << PAGE_SHIFT, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//if (ptr == nullptr)
	//{
	//	throw bad_alloc();
	//}
	void* ptr = SystemAlloc((NPAGES - 1));

	//������ռ�,��СΪ(NPAGES-1)
	Span* maxspan = new Span;
	maxspan->_pageid = (PageID)ptr >> PAGE_SHIFT;
	maxspan->_npage = (NPAGES - 1);
	_pagelist[NPAGES - 1].PushFront(maxspan);

	for (size_t i = 0; i < maxspan->_npage; ++i)
	{
		_id_span_map[maxspan->_pageid + i] = maxspan;
	}
	return _NewSpan(npage);
}

// ��ȡ�Ӷ���span��ӳ��
Span* PageCache::MapObjectToSpan(void* obj)
{
	PageID pageid = (PageID)obj >> PAGE_SHIFT;//ͨ����ַǿת�����Σ���Ϊһҳ=4096�ֽڣ�ҳ���յ�ַ���֣�����4k���ܵõ�ҳ��
	auto it = _id_span_map.find(pageid);
	//unordered_map<PageID, Span*>::iterator it = _id_span_map.find(pageid);
	
	assert(it != _id_span_map.end());
	return it->second;//Span*
}

// �ͷſ���span�ص�PageCache���ϲ����ڵ�span
void PageCache::ReleaseSpanToPageCache(Span* span)
{
//  std::unique_lock<std::mutex> lock(_mtx);

	if (span->_npage >= NPAGES)
	{
		void* ptr = (void*)(span->_pageid << PAGE_SHIFT);
		_id_span_map.erase(span->_pageid);
		SystemFree(ptr,span->_npage);
		delete span;
		return;
	}

	//����PageID���ϲ�span,����ǰ���span�����Һ����ҳ������ٹҵ���Ӧ��С��pagelist��
	auto previt = _id_span_map.find(span->_pageid - 1);
	while (previt != _id_span_map.end())
	{
		if (previt->second->_usecount != 0)
			break;
		
		// ����ϲ�������NPAGESҳ��span���򲻺ϲ�������û�취����
		if (previt->second->_npage + span->_npage >= NPAGES)
			break;

		else
		{
			_pagelist[previt->second->_npage].Erase(previt->second);//�뽫���ó�������
			previt->second->_npage += span->_npage;
			//for (size_t i = 0; i < span->_pagesize; ++i)//ӳ��
			//{
			//	_id_span_map[span->_pageid + i] = previt->second;
			//}
			delete span;//��span����prevspan����
			span = previt->second;
			
			previt = _id_span_map.find(span->_pageid - 1);
		}
	}
	
	auto nextit = _id_span_map.find(span->_pageid +span->_npage);
	while (nextit != _id_span_map.end())
	{
		if (nextit->second->_usecount != 0)
			break;

		// ����ϲ�������NPAGESҳ��span���򲻺ϲ�������û�취����
		if (span->_npage + nextit->second->_npage >= NPAGES)
			break;

		else
		{
			_pagelist[nextit->second->_npage].Erase(nextit->second);
			span->_npage += nextit->second->_npage;
      size_t n = nextit->second->_npage;
			delete nextit->second;

			//for (size_t i = 0; i < n ; ++i)//ӳ��
			//{
   		//	_id_span_map[span->_pageid + span->_pagesize + i] = span;//operator[]ͨ��key�ҵ���key��Ӧ��valueȻ�󷵻�������,�����൱�ڵڶ�������ָ��span
			//}
			nextit = _id_span_map.find(span->_pageid + span->_npage);
		}
	}

	//���ϲ��õ�ҳ��ӳ�䵽�µ�span��
	for (size_t i = 0; i < span->_npage ; i++)
	{
		_id_span_map[span->_pageid + i] = span;
	}
	//����Ӧ��span���ڶ�Ӧ��λ��
	_pagelist[span->_npage].PushFront(span);
}

//// �ͷſ���span�ص�Pagecache�����ϲ����ڵ�span
//void PageCache::ReleaseSpanToPageCache(Span* span)
//{
//	auto previt = _id_span_map.find(span->_pageid - 1);
//	while (previt != _id_span_map.end())
//	{
//		Span* prevspan = previt->second;
//		// ���ǿ��У���ֱ������
//		if (prevspan->_usecount != 0)
//			break;
//
//		// ����ϲ�������NPAGESҳ��span���򲻺ϲ�������û�취����
//		if (prevspan->_pagesize + span->_pagesize >= NPAGES)
//			break;
//
//		_pagelist[prevspan->_pagesize].Erase(prevspan);
//		prevspan->_pagesize += span->_pagesize;
//		delete span;
//		span = prevspan;
//
//		previt = _id_span_map.find(span->_pageid - 1);
//	}
//
//	auto nextit = _id_span_map.find(span->_pageid + span->_pagesize);
//	while (nextit != _id_span_map.end())
//	{
//		Span* nextspan = nextit->second;
//		if (nextspan->_usecount != 0)
//			break;
//
//		if (span->_pagesize + nextspan->_pagesize >= NPAGES)
//			break;
//
//		_pagelist[nextspan->_pagesize].Erase(nextspan);
//		span->_pagesize += nextspan->_pagesize;
//		delete nextspan;
//
//		nextit = _id_span_map.find(span->_pageid + span->_pagesize);
//	}
//
//	for (size_t i = 0; i < span->_pagesize; ++i)
//	{
//		_id_span_map[span->_pageid + i] = span;
//	}
//	_pagelist[span->_pagesize].PushFront(span);
//}
