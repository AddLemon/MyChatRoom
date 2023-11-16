#pragma once

#include "ThreadPool.h"

class Dispatcher
{
public:
	Dispatcher(ThreadPool* tp);
	void dispatch(Json::Value pkt);

private:
	ThreadPool* m_threadPool;
};

