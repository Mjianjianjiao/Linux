#include "NewMalloc.h"
#include "CentralCache.h"
#include "PageCache.h"
void* NewMalloc::Allocate(size_t bytes)
{
	if (bytes > MAXBYTES)
	{
		size_t  roundsize = ClassSize::_Roundup(bytes, 1 << PAGE_SHIFT);  //��Ҫ���뵽ҳ���������� ��ҳΪ������
		size_t  npage = roundsize >> PAGE_SHIFT;

		Span* span = PageCache::GetInstance()->NewSpan(npage);
		void* ptr = (void*)(span->_pageid << PAGE_SHIFT);
		return ptr;
  }
  else{		
  	assert(bytes <= MAXBYTES);
  	size_t size = ClassSize::Roundup(bytes);   //ȷ���������ڴ��С����λ��
    void* start,*end;
    int fetchnum = CentralCache::GetInstance()->FetchRangeObj(start,end,1,size);
    if(fetchnum < -1){
      cout<<"reply malloc failed"<<endl;
      return NULL;
  }
  return start;
  }
}


void  NewMalloc::Deallocate(void* ptr)
{
	Span* span = PageCache::GetInstance()->MapObjectToSpan(ptr);
	size_t size = span->_objsize;
	if (size > MAXBYTES)
	{
		PageCache::GetInstance()->ReleaseSpanToPageCache(span);
	}
	else
	{
  	assert(size <= MAXBYTES);
  	CentralCache::GetInstance()->ReleaseListToSpans(ptr, size);    
	}



}
