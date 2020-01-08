#include "ConcurrentAlloc.h"
#include <stdlib.h>
#include "Common.h"
void TestThreadCache()
{
	//ConcurrentAlloc(10);
	
	std::vector<void*> v;
	int n = 100;
	for (size_t i = 0; i < n; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
		cout << v.back() << endl;
	}
	//cout << endl << endl;

	for (size_t i = 0; i < n; ++i)
	{
		ConcurrentFree(v[i]);
	}
	v.clear();
	
	cout << endl << endl;
	cout << endl << endl;

	for (size_t i = 0; i < n; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
		cout << v.back() << endl;
	}

	for (size_t i = 0; i < n; ++i)
	{
		ConcurrentFree(v[i]);
	}
	v.clear();
	
}

void TestPageCache()
{
//	void* ptr = VirtualAlloc(NULL, (NPAGES - 1) << PAGE_SHIFT, MEM_RESERVE, PAGE_READWRITE);
	void* ptr = (void*)malloc((NPAGES - 1) << PAGE_SHIFT);
	cout << ptr << endl;
	if (ptr == nullptr)
	{
		throw std::bad_alloc();
	}

	PageID pageid = (PageID)ptr >> PAGE_SHIFT;
	cout << pageid << endl;

	void* shiftptr = (void*)(pageid << PAGE_SHIFT);
	cout << shiftptr << endl;
}

int main()
{
	//TestPageCache();
	TestThreadCache();
	return 0;
}
