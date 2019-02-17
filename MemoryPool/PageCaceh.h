#pragma once

#include "Common.h"
#include <map>
class PageCache
{
public:
	static PageCache* GetInstance()
	{
		return &_inst;
	}
	//或取一个新的span
	Span* NewSpan(size_t npage);

	//建立映射
	void CreatePageidToSpanMap(Span* span);
	// 获取从对象到span的映射
	Span* MapObjectToSpan(void* obj);
	// 释放空闲span回到Pagecache，并合并相邻的span
	void ReleaseSpanToPageCahce(Span* span);


private:
	SpanList _pagelist[NPAGES];
private:
	PageCache() = default;
	PageCache(const PageCache&) = delete;
	static PageCache _inst;

	//建立一个span 与页号之间的映射map
	std::map<PageID, Span*> _id_span_map;
};