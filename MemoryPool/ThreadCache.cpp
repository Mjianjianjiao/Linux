#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::FetchFromCentralCache(size_t index, size_t byte)
{
	FreeList* freelist = &_freelist[index];
	//ʵ��һ���������Ĺ�����������һ�δ�����cache ��ȡ���ٸ������
	//ȡ��̫�������˷�
	//ȡ�Ĺ��ٿ��ܻ���Ϊ������̫�̴ﲻ�����غϲ�������
	//�����ƶ��ڴ棬 ���ǿ��Կ��Խ��п��ƣ����������С, �������С��ʱ�򣬶�����ʱ�������������һ�������ܻ�������ص��˷�
	int num_to_move = min(ClassSize::NumMoveSize(byte),freelist->MaxSize());

	// ����ˮλ�ߵ������ƣ�ʹ�䰴��һ�������������������л�ȡ��  ÿ�ζ�ȥ���Լ������Ļ������룬 ���ټ����������������� ���Ч�ʣ� �����ޣ�����ȡ�ö��˷�
	//��һ������Ҫ����max��С
	if (num_to_move == freelist->MaxSize())
	{
		freelist->SetMaxSize(num_to_move + 1);
	}

	void* start, *end;  //��ȡ���õ���10����������һ���������Ҫ������β���
	size_t fetchnum = CentralCache::GetInstance()->FetchRangeObj(start, end, num_to_move, byte);  //������ȡ10�����ܻ�ȡ����Ŀ������������Ҫȷ��ʵ�ʻ�õĿ���
	if (fetchnum == 1)
		return start;   //���ֻ�����һ�飬 ��ֱ�ӷ���ʹ��

	freelist->PushRange(NEXT_OBJ(start), end, fetchnum-1);  //����һ�鷵�ط��䣬�ӵڶ��鿪ʼʣ�µĹҵ����������У�
	
	
	
	
	return start;
}

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAXBYTES);

	// ����ȡ��
	size = ClassSize::Roundup(size);   //ȷ���������ڴ��С����λ��
	size_t index = ClassSize::Index(size); //���ݶ���λ�û�ȡ�±�
	FreeList* freelist = &_freelist[index];  //ȷ��Ҫ��ȡ�ڴ���λ��
	if (!freelist->Empty())
	{
		return freelist->Pop();  //��Ϊ��ȡ��  һ�㶼��ֱ��ȡ  ���� + O��1��
	}
	else
	{
		return FetchFromCentralCache(index, size) ;  //Ϊ�մ����Ķѻ�ȡ�ڴ�
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(size <= MAXBYTES);
	size_t index = ClassSize::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);

	//�������������������һ�δ����Ļ����ƶ�������
	//��ʼ���ն������Ļ���
	if (freelist->Size() > freelist->MaxSize())
	   ListTooLong(freelist, size);
	//�ͷ��߼�������ǿ�������threadcache �ܵ��ֽ�������2M ����ʼ�ͷ�
}




//�г�ȥ���ڴ汻��ɢ��
//�ж�Ҫ�黹�Ķ���������һ��span
//key-value ָ�뵽span����ӳ�� �����ܻὫ�ڴ�ű�
//ҳ�� �� span ��ӳ��


void ThreadCache::ListTooLong(FreeList* freelist, size_t bytes)
{
	//�����������µ�������գ�ȡ��
	void* start = freelist->Clear();
	//�黹������Cache
	CentralCache::GetInstance()->ReleaseListToSpans(start, bytes);
}

