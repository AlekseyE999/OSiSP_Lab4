#include "TaskQueue.h"
TaskQueue::TaskQueue() {
	InitializeCriticalSection(&lock);
}
void TaskQueue::AddTask(Task task) {
	EnterCriticalSection(&lock);
	tasks.push(task);
	LeaveCriticalSection(&lock);
}
Task TaskQueue::RemoveTask() {
	EnterCriticalSection(&lock);
	Task task;
	if (tasks.size() > 0) {
		task = tasks.front();
		tasks.pop();
	}
	LeaveCriticalSection(&lock);
	return task;
}

int TaskQueue::Size() {
	EnterCriticalSection(&lock);
	auto size = this->tasks.size();
	LeaveCriticalSection(&lock);
	return size;
}