#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::FetchFromCentralCache(size_t index, size_t byte)
{
	FreeList* freelist = &_freelist[index];
	//实现一个慢增长的过程来开决定一次从中心cache 获取多少个对象块
	//取的太多会造程浪费
	//取的过少可能会因为，链表太短达不到返回合并的条件
	//批量移动内存， 但是可以可以进行控制，避免过大或过小, 如果对象小的时候，对象大的时侯申请的数量都一样，可能会造成严重的浪费
	int num_to_move = min(ClassSize::NumMoveSize(byte),freelist->MaxSize());

	
	void* start, *end;  //获取所得到的10个块是连在一起的所以需要，将首尾标出
	size_t fetchnum = CentralCache::GetInstance()->FetchRangeObj(start, end, num_to_move, byte);  //期望获取10个可能获取的数目并不够，所以要确定实际获得的块数
	if (fetchnum == 1)
		return start;   //如果只获得了一块， 就直接返回使用

	freelist->PushRange(NEXT_OBJ(start), end, fetchnum-1);  //将第一块返回分配，从第二块开始剩下的挂到空闲链表中，
	
	//进行水位线调整控制，使其按照一个慢的增长趋势来进行获取、
	//下一次申请要申请max大小
	if (num_to_move == freelist->MaxSize())
	{
		freelist->SetMaxSize(num_to_move + 1);
	}
	
	printf("%s \n",start);
	return start;
}

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAXBYTES);

	// 对齐取整
	size = ClassSize::Roundup(size);   //确定所申请内存大小对齐位置
	size_t index = ClassSize::Index(size); //根据对齐位置获取下标
	FreeList* freelist = &_freelist[index];  //确定要获取内存块的位置
	if (!freelist->Empty())
	{
		return freelist->Pop();  //不为空取出
	}
	else
	{
		return FetchFromCentralCache(index, size) ;  //为空从中心堆获取内存
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(size <= MAXBYTES);
	size_t index = ClassSize::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);

	//当自由链表的数量超过一次从中心缓存移动的数量
	//开始回收对象到中心缓存
	if (freelist->Size() >= freelist->MaxSize())
	   ListTooLong(freelist, size);
	//释放逻辑可以增强：的如果threadcache 总的字节数超过2M ，开始释放
}




//切出去的内存被分散的
//判断要归还的对象属于哪一个span
//key-value 指针到span进行映射 但可能会将内存撑爆
//页号 到 span 的映射


void ThreadCache::ListTooLong(FreeList* freelist, size_t bytes)
{
	//将空闲链表下的链表清空，取出
	void* start = freelist->Clear();
	//归还给中心Cache
	CentralCache::GetInstance()->ReleaseListToSpans(start, bytes);
}

