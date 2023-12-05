#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	//Instantiate task queue
	taskQ = new TaskQueue;
	if (taskQ == nullptr) {
		cout << "new taskQ failed..." << endl;
		return;
	}

	//initialize thread variables
	minNum = THREAD_MIN_NUMBER;
	maxNum = THREAD_MAX_NUMBER;
	busyNum = 0;
	aliveNum = THREAD_MIN_NUMBER;
	exitNum = 0;

	shutdown = false;

	//Create thread
	managerThread = thread(managerFunc, this);
	for (int i = 0; i < THREAD_MIN_NUMBER; ++i) {
		thread thread(workerFunc, this);
		thread.detach();
	}
}

ThreadPool::~ThreadPool()
{
	cout << "start to destroy thread pool.." << endl;			//test********
	//Close thread pool
	shutdown = true;
	//Block to wait manager thread exit
	managerThread.join();
	//Wake up blocked worker threads
	for (int i = 0; i < aliveNum; ++i) {
		notEmpty.notify_all();
		this_thread::sleep_for(chrono::milliseconds(50));		//to ensure that the awakened thread has finished exiting
	}
	//free heap memory
	if (taskQ) {
		delete taskQ;
		taskQ = nullptr;
	}
	cout << "complete to destroy thread pool.." << endl;		//test********
}

void ThreadPool::addTask(Command* task)
{
	if (shutdown) {
		return;
	}

	//Add task
	taskQ->addTask(task);	//The lock is encapsulated internally
	notEmpty.notify_all();
}

int ThreadPool::getBusyNum()
{
	//lock
	unique_lock<mutex> lock(mutexPool);		//Configure the lock object, which is locked by default when completed.
	int busyNum = this->busyNum;
	//unlock
	lock.unlock();
	return busyNum;
}

int ThreadPool::getAliveNum()
{
	//lock
	unique_lock<mutex> lock(mutexPool);		//Configure the lock object, which is locked by default when completed.
	int aliveNum = this->aliveNum;
	//unlock
	lock.unlock();
	return aliveNum;
}

void ThreadPool::workerFunc(void* arg)
{
	//Conversion parameter type
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	unique_lock<mutex> lock(pool->mutexPool, defer_lock);	//Configure the lock object and it is in the unlocked state

	while (true) {
		//lock
		lock.lock();

		//Determine whether the current task queue is empty
		while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
			//Block worker thread
			pool->notEmpty.wait(lock);

			//Determine whether to destroy the thread
			if (pool->exitNum > 0) {
				pool->exitNum--;
				if (pool->aliveNum > pool->minNum) {
					pool->aliveNum--;
					lock.unlock();		//Because the blocked thread regained the lock after being awakened
					pool->threadExit();	//thread exit
					return;
				}
			}
		}

		//Determine whether the thread pool is closed
		if (pool->shutdown) {
			lock.unlock();		
			pool->threadExit();	
			return;
		}

		//take a task from the task list
		Command* task = pool->taskQ->takeTask();
		pool->busyNum++;

		cout << "thread " << this_thread::get_id() << " start working..." << endl;

		//unlock
		lock.unlock();

		//Execute function
		task->execute();
		task->send();
		delete task;

		lock.lock();
		cout << "thread " << this_thread::get_id() << " end working..." << endl;
		pool->busyNum--;
		lock.unlock();
	}
}

void ThreadPool::managerFunc(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	unique_lock<mutex> lock(pool->mutexPool, defer_lock);		//Configure the lock object and it is in the unlocked state

	while (!pool->shutdown) {
		//Check every 3s
		this_thread::sleep_for(std::chrono::seconds(3));

		//Get the number of tasks in the thread pool, the number of alive threads, and the number of busy threads
		lock.lock();
		int queueSize = pool->taskQ->taskNumber();
		int liveNum = pool->aliveNum;
		int busyNum = pool->busyNum;
		lock.unlock();

		//Add thread
		//Rule: ( Number of tasks > Number of alive threads ) && ( Number of alive threads < Maximum number of threads )
		if (queueSize > liveNum && liveNum < pool->maxNum) {
			lock.lock();
			int counter = 0;
			for (int i = 0; i < pool->maxNum && counter < THREAD_DESTROY_NUMBER && pool->aliveNum < pool->maxNum; ++i) {
				thread thread(workerFunc, pool);
				thread.detach();
				counter++;
				pool->aliveNum++;
			}
			lock.unlock();
		}

		//Destroy thread
		//Rule: ( Busy threads*2 < Number of surviving threads) && ( Number of surviving threads > Minimum number of threads )
		if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
			lock.lock();
			pool->exitNum = THREAD_DESTROY_NUMBER;
			lock.unlock();

			//Let the blocked thread exit
			for (int i = 0; i < THREAD_DESTROY_NUMBER; ++i) {
				pool->notEmpty.notify_all();	//Wake up blocked threads
			}
		}
	}
}

void ThreadPool::threadExit()
{
	unique_lock<mutex> lock(mutexPool);		//lock

	cout << "threadExit() called, " << this_thread::get_id() << " exiting..." << endl;
}