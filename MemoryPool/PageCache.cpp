#include "PageCaceh.h"

PageCache PageCache::_inst;

//为什么不直接去系统中申请2页，跳过第三层
//频繁申请过小的空间，会早成效率的降低

void PageCache::CreatePageidToSpanMap(Span* span)
{
	for (int i = 0; i < span->_npage; ++i)
	{
		_id_span_map[span->_pageid + i] = span;
	}
}

Span* PageCache::NewSpan(size_t npage)
{
	if (!_pagelist[npage].Empty())
	{
		return _pagelist[npage].PopFront();
	}

	for (size_t i = npage+1; i < NPAGES; ++i)
	{
		SpanList* pagelist = &_pagelist[i];
		//对pagelist对应的桶进行加锁
		std::unique_lock<std::mutex> lock(pagelist->_mtx);
		
		if (!pagelist->Empty())                          //循环遍历后面的page ,如果有不为空的就取其中的一个span 将
		{
			Span* span = pagelist->PopFront();
			Span* split = new Span;
			split->_pageid = span->_pageid + span->_npage - npage;
			split->_npage = npage;
			span->_npage -= npage;

			_pagelist[span->_npage].PushFront(span);

			//建立分配的块与span的页号之间的关系
			CreatePageidToSpanMap(split);

			return split;
		}
	}

	// 需要向系统申请内存  内存的上限128 不是固定的，是自定义的
	void* ptr = VirtualAlloc(NULL, (NPAGES-1)<<PAGE_SHIFT, MEM_RESERVE| MEM_COMMIT , PAGE_READWRITE);  //最大页数是129 的原因是因为跳过了0 
	if (ptr == nullptr)
	{
		throw std::bad_alloc();
	}

	Span* largespan = new Span;
	largespan->_pageid = (PageID)ptr >> PAGE_SHIFT;
	largespan->_npage = NPAGES - 1;

	_pagelist[NPAGES - 1].PushFront(largespan);
	
     //将从内存分配的页号也与span进行映射
	CreatePageidToSpanMap(largespan);

    
	return NewSpan(npage);
}


Span* PageCache::MapObjectToSpan(void* obj)
{
	//算出页号
	PageID pageid = (PageID)obj >> PAGE_SHIFT;
	//根据pageid 找到对应的span的迭代器
	auto it = _id_span_map.find(pageid);
	return it->second;
}


//双向的带头的循环链表方便插入删除等的逻辑
//将span 进行合并
void PageCache::ReleaseSpanToPageCahce(Span* span){
	
	//找到当前的要插入的页的前一页的span
	auto previt = _id_span_map.find(span->_pageid - 1);
	SpanList* pagelist = &PageCache::GetInstance()->_pagelist[previt->second->_npage];
	std::unique_lock<std::mutex> lock_prev(pagelist->_mtx);
	while (previt->second != pagelist->begin()){
		if (previt->second->_usecount != 0)
			break;
		if (span->_npage >= 128)//合并时要进行控制，可能会合并出一个特别大的页，不能超过128
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
	std::unique_lock<std::mutex> lock_next(pagelist->_mtx);
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
	//合并后进行map的更新
	CreatePageidToSpanMap(span);
	//插入
	PageCache::GetInstance()->_pagelist[span->_npage].PushFront(span);
	
}