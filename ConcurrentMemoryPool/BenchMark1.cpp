#define _CRT_SECURE_NO_WARNINGS 1

#include"ConcurrentAlloc.h"
#include<iostream>
#include<vector>
#include<stdio.h>

void BenchmarkMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;
	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&, k]() {
			std::vector<void*> v;
			v.reserve(ntimes);
			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(malloc(129*4 * 1024));
				}
				size_t end1 = clock();
				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					free(v[i]);
				}
				size_t end2 = clock();
				v.clear();
				malloc_costtime += end1 - begin1;
				free_costtime += end2 - begin2;
			}
		});
	}
	for (auto& t : vthread)
	{
		t.join();
	}
	printf("%zd个线程并发执行%zd轮次，每轮次malloc %zd次: 花费：%zd ms\n", nworks, rounds, ntimes, malloc_costtime);
	printf("%zd个线程并发执行%zd轮次，每轮次free %zd次: 花费：%zd ms\n",
		nworks, rounds, ntimes, free_costtime);
	printf("%zd个线程并发malloc&free %zd次，总计花费：%zd ms\n",
		nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}
// 单轮次申请释放次数 线程数 轮次
void BenchmarkConcurrentMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;
	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&]() {
			std::vector<void*> v;
			v.reserve(ntimes);
			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(ConcurrentAlloc(129*4*1024));
				}
				size_t end1 = clock();
				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					ConcurrentFree(v[i]);
					//cout << i << endl;
				}
				size_t end2 = clock();
				v.clear();
				malloc_costtime += end1 - begin1;
				free_costtime += end2 - begin2;
			}
		});
	}
	for (auto& t : vthread)
	{
		t.join();
	}
	printf("%zd个线程并发执行%zd轮次，每轮次concurrent alloc %zd次: 花费：%zd ms\n",nworks, rounds, ntimes, malloc_costtime);
	printf("%zd个线程并发执行%zd轮次，每轮次concurrent dealloc %zd次: 花费：%zd ms\n",
		nworks, rounds, ntimes, free_costtime);
	printf("%zd个线程并发concurrent alloc&dealloc %zd次，总计花费：%zd ms\n",
		nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}
int main()
{
	cout << "==========================================================" << endl;
  BenchmarkMalloc(10, 100, 100);
	cout << endl << endl;
	BenchmarkConcurrentMalloc(10, 100, 100);
	cout << "==========================================================" << endl;

	return 0;
}
