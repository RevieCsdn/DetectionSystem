#pragma once
#include <iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<functional>
#include<queue>
#include <list>
#define N 10
using namespace std;

class MyThreadPool {

public:
	//自定义void()的函数类型
	typedef function<void()> Task;

	MyThreadPool();
	~MyThreadPool();

public:
	size_t m_initNum;
	//线程数组
	vector<thread> m_threads;

	//任务队列
	list<Task> m_tasks;

	//互斥锁条件变量
	mutex m_mutex;
	condition_variable m_cond;

	mutex m_mutex1;
	condition_variable m_cond1;
	//线程池工作结束时为真
	bool m_done;

	//队列是否为空
	bool m_isEmpty;
	//队列是否为满
	bool m_isFull;

	int m_runNum;
	int m_exit;
public:
	void AddTask(const Task& f);
	void Start(int num);
	void SetSize(int num);
	void RunTask();
	void Finish();

	void SetRunNum(int num);
	void SetExitThread(int num);
};
