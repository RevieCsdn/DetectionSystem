
#include "MyThreadPool.h"
#include <process.h>

MyThreadPool::MyThreadPool()
	:m_done(false),m_isEmpty(true),m_isFull(false)
{
	m_runNum = 0;
	m_exit = 0;
}

MyThreadPool::~MyThreadPool()
{
	//if (!m_threads.empty())
	//{
	//	m_cond.notify_all();
	//}
	//for (size_t i = 0; i < m_threads.size(); i++) {
	//	m_threads[i].join();
	//}
	if (!m_tasks.empty())
	{
		m_tasks.clear();
	}
	if (!m_threads.empty())
	{
		m_threads.clear();
	}

}
//生产者
void MyThreadPool::AddTask(const Task& f)
{
	if (!m_done) 
	{
		//保护共享资源    
		unique_lock<mutex> l_lock(m_mutex);

		//要是任务数量到了最大,就等待处理完再添加
		while (m_isFull) {
			m_cond.wait(l_lock);
		}

		//给队列中添加任务
		m_tasks.push_back(f);

		if (m_tasks.size() == m_initNum)
		{
			m_isFull = true;
		}

		//添加完任务队列必不为空
		m_isEmpty = false;
	
	//cout << "Add a task" << endl;
		l_lock.unlock();
		m_cond.notify_one();
	}
}

void MyThreadPool::Start(int num)
{
	m_done = false;
	SetSize(num);

	for (int i = 0; i < num; i++) {
		m_threads.push_back(thread(&MyThreadPool::RunTask, this));
	}

}

void MyThreadPool::SetSize(int num)
{
	this->m_initNum = num;
}

//消费者
void MyThreadPool::RunTask()
{
	//不断遍历队列,判断要是有任务的话,就执行
	while (!m_done) 
	{
		unique_lock<mutex> l_lock(m_mutex);
		//队列为空的话,就等待任务
		while (m_isEmpty) {
			m_cond.wait(l_lock);
		}
	
		Task l_task;
		l_task = move(m_tasks.front());
		m_tasks.pop_front();

		if (m_tasks.empty())
		{
			m_isEmpty = true;
			if (m_runNum == m_exit)
			{
				cout << "===Exit threadPool===" << endl;
				m_done = true;
			}
			
		}
		//消费完任务队列必不满 
		m_isFull = false;
		l_lock.unlock();

		l_task();
		m_cond.notify_one();

	}
}

void MyThreadPool::Finish()
{
	//for (size_t i = 0; i < m_threads.size(); i++) {
	//	m_threads[i].join();
	//}

	for (auto& item: m_threads)
	{
		if (item.joinable())
		{
			item.join();
		}
	}
	//for (auto& item: m_threads)
	//{
	//	if (item->joinable())
	//	{
	//		item->join();
	//	}
	//}
}

void MyThreadPool::SetRunNum(int num)
{
	m_runNum = num;
}

void MyThreadPool::SetExitThread(int num)
{
	m_exit = num;
}
