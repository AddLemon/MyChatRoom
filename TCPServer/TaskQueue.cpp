#include "TaskQueue.h"

void TaskQueue::addTask(Task* task)
{
	//lock
	unique_lock<mutex> lock(m_mutex);	//lock

	m_taskQ.push(task);
}

Task* TaskQueue::takeTask()
{
	Task* task = nullptr;
	//lock
	unique_lock<mutex> lock(m_mutex);

	if (!m_taskQ.empty()) {
		task = m_taskQ.front();
		m_taskQ.pop();
	}

	return task;
}

