#pragma once
#include <Windows.h>
#include <thread>
using namespace std;

/*
创建线程	CreateThread
销毁线程	DestoryThread
等待线程	WaitForThread
*/

// 回调函数类型void myFunction(BF_Thread* thd)

class BF_Thread
{
public:
	// CreateThread参数 回调函数类型 void myFunction(BF_Thread* thd)
	BOOL CreateThread(void (*callBack)(BF_Thread*));	// 创建线程		// callBack为函数指针 填函数名
	void DestoryThread();										// 销毁线程
	void WaitForThread();										// 等待线程

public:
	BF_Thread();
	~BF_Thread();
public:
	// 用于线程线程循环执行，true继续循环运行，false停止循环运行
	bool continueRunning;

private:
	thread* ThreadPtr;
	// thread actualThread;
};

BF_Thread::BF_Thread()
{
	this->continueRunning = false;
	this->ThreadPtr = nullptr;
}
BOOL BF_Thread::CreateThread(void (*callBack)(BF_Thread*))
{
	if (ThreadPtr != nullptr){
		return false;
	}

	ThreadPtr = new thread(callBack, this);
	// 以下三种写法全错了
	//thread actualThread(callBack);
	//ThreadPtr = &actualThread;
	//ThreadPtr = &actualThread;

	if (ThreadPtr != nullptr) {
		continueRunning = true;
		return true;
	}
	return false;
}
void BF_Thread::DestoryThread()
{
	continueRunning = false;
}
void BF_Thread::WaitForThread()
{
	ThreadPtr->join();
}
BF_Thread::~BF_Thread()
{
	if (ThreadPtr != nullptr)
		delete ThreadPtr;
}

// 案例:
/*
* void aaa(BF_Thread* thd) {
    while (thd->continueRunning)
    {
        printf("123\n");
        Sleep(200);
    }
}
int main()
{
	BF_Thread thd;
	thd.CreateThread(aaa);
	Sleep(3000);
	thd.continueRunning = false;
	thd.WaitForThread();
	return 0;
}
*/
