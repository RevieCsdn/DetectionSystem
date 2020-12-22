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
	//�Զ���void()�ĺ�������
	typedef function<void()> Task;

	MyThreadPool();
	~MyThreadPool();

public:
	size_t m_initNum;
	//�߳�����
	vector<thread> m_threads;

	//�������
	list<Task> m_tasks;

	//��������������
	mutex m_mutex;
	condition_variable m_cond;

	mutex m_mutex1;
	condition_variable m_cond1;
	//�̳߳ع�������ʱΪ��
	bool m_done;

	//�����Ƿ�Ϊ��
	bool m_isEmpty;
	//�����Ƿ�Ϊ��
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
