#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <sys/mman.h>
using std::cout;
using std::endl;

#include <assert.h>

// 管理对象自由链表的长度
const size_t NLISTS = 240;  //?
const size_t MAXBYTES = 64 * 1024;
const size_t PAGE_SHIFT = 12;
const size_t NPAGES = 129;

static inline void*& NEXT_OBJ(void* obj)
{
	return *((void**)obj);
}

static void* SystemAlloc(size_t npage){
//按页向系统申请
  void* ptr; 
  ptr = mmap(NULL ,128*4096 , PROT_WRITE|PROT_READ,  MAP_ANON | MAP_PRIVATE,-1,0);
  if(ptr == (void*)-1){
    cout<<"errno:" <<errno<<endl;
     throw std::bad_alloc();
   }
   
   return ptr;
}

static void* SystemFree(void* ptr,size_t npage){
  munmap(ptr,npage*4096);
}


typedef size_t PageID;
struct Span
{
	PageID _pageid = 0;			// 页号
	size_t _npage = 0;		// 页的数量

	Span*  _next = nullptr;
	Span*  _prev = nullptr;

	void*  _objlist = nullptr;	// 对象自由链表
	size_t _objsize = 0;		// 对象大小
	size_t _usecount = 0;		// 使用计数
};

class SpanList
{
public:
	SpanList()
	{
    //初始化构造span 链表，双向循环链表
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}

	Span* begin()
	{
		return _head->_next;
	}

	Span* end()
	{
		return _head;
	}

	bool Empty()
	{
		return _head->_next == _head;
	}

	void Insert(Span* cur, Span* newspan)
	{
		assert(cur);
		Span* prev = cur->_prev;
		// 前插

		prev->_next = newspan;
		newspan->_prev = prev;
		newspan->_next = cur;
		cur->_prev = newspan;
	}

	void Erase(Span* cur)
	{
		assert(cur != nullptr && cur != _head);
		Span* prev = cur->_prev;
		Span* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
	}

	void PushFront(Span* span)
	{
		Insert(begin(), span);
	}

	void PushBack(Span* span){
		Insert(end(), span);
	}

	void* PopBack(){
		Span* end = _head->_prev;
		Erase(end);
		return end;
	}

	Span* PopFront()
	{
		Span* span = begin();
		Erase(span);
		return span;
	}

//属于spanlist的锁
	std::mutex _mtx;
private:
	Span* _head = nullptr;

};

class ClassSize
{
	
public:
	static inline size_t _Roundup(size_t size, size_t align)
	{
		return (size + align - 1) & ~(align - 1);
	}

	static inline size_t Roundup(size_t size)
	{
		assert(size <= MAXBYTES);

		if (size <= 128) {
			return _Roundup(size, 8);
		}
		else if (size <= 1024){
			return _Roundup(size, 16);
		}
		else if (size <= 8192){
			return _Roundup(size, 128);
		}
		else if (size <= 65536){
			return _Roundup(size, 512);
		}
		else {
			return -1;
		}
	}

	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
	}

	static inline size_t Index(size_t bytes)
	{
		assert(bytes <= MAXBYTES);

		// 每个区间有多少个自由链表
		static int group_array[4] = { 16, 56, 56, 112 };

		if (bytes <= 128){
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024){
			return _Index(bytes - 128, 4) + group_array[0];
		}
		else if (bytes <= 8192){
			return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (bytes <= 65536){
			return _Index(bytes - 8192, 9) + group_array[2] + group_array[1] +
				group_array[0];
		}
		else{
			return -1;
		}
	}



	static size_t NumMoveSize(size_t size)
	{
		//控制分配的内存，避免太多或太少
		if (size == 0)
			return 0;
		//批量移动内存， 但是可以进行控制
		int num = static_cast<int>(MAXBYTES / size);
		if (num < 2)
			num = 2;

		if (num > 512)
			num = 512;

		return num;
	}

	// 计算一次向系统获取几个页
	static size_t NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);
		size_t npage = num*size;

		npage >>= 12;
		if (npage == 0)
			npage = 1;

		return npage;
	}
};
