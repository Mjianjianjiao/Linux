#define _CRT_SECURE_NO_WARNINGS 1
#include"CentralCache.h"
#include"PageCache.h"
#include"NewMalloc.h"
#include <time.h>
void TestNewMalloc1()
{

  
	std::vector<void*> v;
  clock_t start, end;
  start = clock();
	for (size_t i = 0; i < 100000; ++i)
	{

    int randx = rand()%(64*1023)+1;

		v.push_back(NewMalloc::Allocate(randx));
	//	cout << v.back() << endl;
	}
  end = clock();
  double time = (double)(end - start);
  cout<<"NewMalloc："<<endl<<"申请时间："<<time<<endl;

  start = clock();
 for (size_t i = 0; i < 100000; ++i)
	{
    NewMalloc::Deallocate(v[i]);
	}
  end = clock();
  time = (double)(end - start);
  cout<<"释放时间："<<time<<endl;

	v.clear();
}


int main(){
  TestNewMalloc1();
//  TestPageCache();
  return 0;
}


