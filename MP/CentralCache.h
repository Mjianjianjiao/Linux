#pragma once

#include "Common.h"

class CentralCache
{
public:
	static CentralCache* GetInstance(){  //���Ķ�ֻ��һ��ʹ�õ���ģʽ������ģʽ �� �����µȴ�ʱ�䳤������ģʽ�ļ������� ��˫�ؼ��ή��Ч��
		return &_inst;
	}

	Span* GetOneSpan(SpanList* spanlist, size_t bytes);

	// �����Ļ����ȡһ�������Ķ����thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t num, size_t index);

	// ��һ�������Ķ����ͷŵ�span���
	void ReleaseListToSpans(void* start, size_t byte_size);


private:
	// ���Ļ�����������
	SpanList _spanlist[NLISTS];
private:
	CentralCache() = default;
	CentralCache(const CentralCache&) = delete;
	CentralCache& operator=(const CentralCache&) = delete;

	static CentralCache _inst;
};
