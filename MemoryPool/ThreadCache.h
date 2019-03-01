#pragma once

#include "Common.h"

class ThreadCache
{
public:
	//������Դ
	void* Allocate(size_t size);
	//�ͷ���Դ
	void Deallocate(void* ptr, size_t size);

	// �����Ļ����ȡ����
	void* FetchFromCentralCache(size_t index, size_t size);

	//����̫�������л���
	void ListTooLong(FreeList* freelist, size_t bytes);
private:
	//�ڴ���С�Ĺ�ϣӳ���
	FreeList _freelist[NLISTS];
	//int _tid;
	//ThreadCache* _next;
};

//Ϊÿһ���̴߳���һ�����Ե�ȫ�ֱ���
//��̬   ���� �������
static _declspec(thread) ThreadCache* tls_threadcache = nullptr;
//�������е�ϵͳ��֧�֣� Ҫʹ�ö�̬