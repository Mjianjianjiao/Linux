#pragma once

#include "Common.h"

class CentralCache
{
public:
	static CentralCache* GetInstance(){  //中心堆只有一个使用单例模式，饿汉模式 ， 不用怕等待时间长，懒汉模式的加锁解锁 与双重检查会降低xiaolv
		return &_inst;
	}

	Span* GetOneSpan(SpanList* spanlist, size_t bytes);

	// 从中心缓存获取一定数量的对象给thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte);

	// 将一定数量的对象释放到span跨度
	void ReleaseListToSpans(void* start, size_t byte_size);
private:
	// 中心缓存自由链表
	SpanList _spanlist[NLISTS];
private:
	CentralCache() = default;
	CentralCache(const CentralCache&) = delete;
	CentralCache& operator=(const CentralCache&) = delete;


	static CentralCache _inst;


};