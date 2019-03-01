#include "PageCaceh.h"

PageCache PageCache::_inst;

//Ϊʲô��ֱ��ȥϵͳ������2ҳ������������
//Ƶ�������С�Ŀռ䣬�����Ч�ʵĽ���

void PageCache::CreatePageidToSpanMap(Span* span)
{
	for (size_t  i = 0; i < span->_npage; ++i)
	{
		_id_span_map[span->_pageid + i] = span;
	}
}


Span* PageCache::_NewSpan(size_t npage)
{

	if (!_pagelist[npage].Empty())
	{
		return _pagelist[npage].PopFront();
	}

	for (size_t i = npage+1; i < NPAGES; ++i)
	{

		SpanList* pagelist = &_pagelist[i];
		
		
		if (!pagelist->Empty())                          //ѭ�����������page ,����в�Ϊ�յľ�ȡ���е�һ��span ��
		{
			Span* span = pagelist->PopFront();
			Span* split = new Span;
			split->_pageid = span->_pageid + span->_npage - npage;
			split->_npage = npage;
			span->_npage -= npage;

			_pagelist[span->_npage].PushFront(span);

			//��������Ŀ���span��ҳ��֮��Ĺ�ϵ
			CreatePageidToSpanMap(split);

			split->_objsize = npage << PAGE_SHIFT;
			return split;
		}


	}

	// ��Ҫ��ϵͳ�����ڴ�  �ڴ������128 ���ǹ̶��ģ����Զ����
	void* ptr = SystemAlloc(npage);

	Span* largespan = new Span;
	largespan->_pageid = (PageID)ptr >> PAGE_SHIFT;
	largespan->_npage = NPAGES - 1;

	_pagelist[NPAGES - 1].PushFront(largespan);
	
     //�����ڴ�����ҳ��Ҳ��span����ӳ��
	CreatePageidToSpanMap(largespan);

	return _NewSpan(npage);    
}

Span* PageCache::NewSpan(size_t npage)
{

	
	std::unique_lock<std::mutex> lock(page_mtx);

	//��pagelist��Ӧ��Ͱ���м���
	//����ĵݹ���ܻ��������, ����ʹ���Ӻ���

	if (npage >= NPAGES)
	{ //��������ҳ������128ҳ��ֱ�Ӵ�ϵͳ��ȡ
		void* ptr = SystemAlloc(npage);
		Span* span = new Span;
		span->_pageid = (PageID)ptr >> PAGE_SHIFT;
		span->_npage = npage;
		span->_objsize = npage << PAGE_SHIFT;
		_id_span_map[span->_pageid] = span;

		return span;
	}
	return _NewSpan(npage); //�Ӻ������������

}

Span* PageCache::MapObjectToSpan(void* obj)
{
	//���ҳ��
	PageID pageid = (PageID)obj >> PAGE_SHIFT;
	//����pageid �ҵ���Ӧ��span�ĵ�����
	auto it = _id_span_map.find(pageid);
	return it->second;
}


//˫��Ĵ�ͷ��ѭ�����������ɾ���ȵ��߼�
//��span ���кϲ�
void PageCache::ReleaseSpanToPageCahce(Span* span){
	
	std::unique_lock<std::mutex> lock(page_mtx);
	if (span->_npage >= NPAGES){
		void* ptr = (void*)(span->_pageid << PAGE_SHIFT);
		SystemFree(ptr);
		delete span;
		return;
	}

	//�ҵ���ǰ��Ҫ�����ҳ��ǰһҳ��span
	auto previt = _id_span_map.find(span->_pageid - 1);
	SpanList* pagelist = &PageCache::GetInstance()->_pagelist[previt->second->_npage];
	while (previt->second != pagelist->begin()){
		if (previt->second->_usecount != 0)
			break;
		if (span->_npage >= 128)//�ϲ�ʱҪ���п��ƣ����ܻ�ϲ���һ���ر���ҳ�����ܳ���128
			break;

		pagelist->Erase(previt->second);
		span->_pageid = previt->second->_pageid;
		span->_npage += previt->second->_npage;
		delete previt->second;
		
		previt = _id_span_map.find(span->_pageid - 1);
		SpanList* pagelist = &PageCache::GetInstance()->_pagelist[previt->second->_npage];
	}

	auto nextit = _id_span_map.find(span->_pageid + span->_npage);
	pagelist = &PageCache::GetInstance()->_pagelist[nextit->second->_npage];
	while (nextit->second != pagelist->end()){

		if (nextit->second->_usecount != 0)
			break;
		if (span->_npage >= 128)
			break;

		pagelist->Erase(nextit->second);
		span->_npage += nextit->second->_npage;
		delete nextit->second;

		nextit = _id_span_map.find(span->_pageid + span->_npage);
		pagelist = &PageCache::GetInstance()->_pagelist[nextit->second->_npage];
	}
	//�ϲ������map�ĸ���
	CreatePageidToSpanMap(span);
	//����
	PageCache::GetInstance()->_pagelist[span->_npage].PushFront(span);
	
}