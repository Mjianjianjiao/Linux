#pragma once
#include "Epoll.h"

class Sock5Server : public EpollServer
{
public:
	Sock5Server(int port)
		:EpollServer(port)
	{}

	int AuthHandle(int fd);
	int EstablishmentHandle(int fd);

	virtual void ConnectEventHandle(int connectfd);
	virtual void ReadEventHandle(int connectfd);

	//virtual void WriteEventHandle(int connectfd);

protected:

};

