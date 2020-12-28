
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
//������
void MyThreadPool::AddTask(const Task& f)
{
	if (!m_done) 
	{
		//����������Դ    
		unique_lock<mutex> l_lock(m_mutex);

		//Ҫ�����������������,�͵ȴ������������
		while (m_isFull) {
			m_cond.wait(l_lock);
		}

		//���������������
		m_tasks.push_back(f);

		if (m_tasks.size() == m_initNum)
		{
			m_isFull = true;
		}

		//�����������бز�Ϊ��
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

//������
void MyThreadPool::RunTask()
{
	//���ϱ�������,�ж�Ҫ��������Ļ�,��ִ��
	while (!m_done) 
	{
		unique_lock<mutex> l_lock(m_mutex);
		//����Ϊ�յĻ�,�͵ȴ�����
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
		//������������бز��� 
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
