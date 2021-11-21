#pragma once
#include <Windows.h>
#include <string>;
#include <queue>
#include "Task.h"
using namespace std;

class TaskQueue {

private:
	CRITICAL_SECTION lock;
	queue<Task> tasks;

public:
	TaskQueue();
	void AddTask(Task task);
	Task RemoveTask();
	int Size();
};