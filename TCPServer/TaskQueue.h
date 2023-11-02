#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
using namespace std;

//task structure
class Task {
public:
	/*template <typename Obj, typename Func, typename... Args>
	Task(Obj obj, Func f, Args... args) {
		m_func = bind(obj, f, forward<Args>(args)...);
	}*/
	template <typename Func, typename... Args>
	Task(Func f, Args... args) {
		m_func = bind(f, forward<Args>(args)...);
	}
	void run() {
		m_func();
	}
private:
	function<void()> m_func;
};

class TaskQueue
{
private:
	queue<Task*> m_taskQ;
	mutex m_mutex;
public:
	//Add task
	void addTask(Task* task);

	//Take out a task
	Task* takeTask();

	//Get the current number of tasks
	size_t taskNumber() {
		return m_taskQ.size();
	}
};
