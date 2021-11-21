#include <fstream>

#include "TaskQueue.h"

#define THREAD_COUNT 5

TaskQueue* taskQueue;
TaskQueue* resultQueue;

vector<string> ReadFromFile();
void WriteToResultFile(const vector<string>& data);
void CreateThreadsPool(int threadCount);
void CreateTasks(vector<string>& data);
DWORD WINAPI ProcessTask();
vector<string> MergeSort(vector<string> v1, vector<string> v2);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR args, int nCmdShow)
{
	taskQueue = new TaskQueue();
	resultQueue = new TaskQueue();
	auto data = ReadFromFile();
	CreateTasks(data);
	CreateThreadsPool(THREAD_COUNT);
	vector<string> result;
	if (resultQueue->Size() > 0) {
		result = taskQueue->RemoveTask().text;
		while (resultQueue->Size() > 0) {
			data = resultQueue->RemoveTask().text;
			result = MergeSort(result, data);
		}
		WriteToResultFile(result);
	}
	free(taskQueue);
	free(resultQueue);
	return 0;
}

vector<string> ReadFromFile() {
	ifstream file("data.txt");
	string word;
	vector<string> data;
	while (file >> word) {
		data.push_back(word);
	}
	return data;
}

void WriteToResultFile(const vector<string>& data) {
	ofstream file("OutPut.txt");
	for (const auto& line : data) {
		file << line << endl;
	}
}

void CreateThreadsPool(int threadCount) {
	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProcessTask, NULL, 0, NULL);
	}
	WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
	for (int i = 0; i < threadCount; i++)
	{
		if (threads[i] != NULL)
		{
			threads[i] = NULL;
		}
	}
	free(threads);
}

void CreateTasks(vector<string>& data) {
	int taskSize = ceil((double)data.size() / THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; i++) {
		Task task(vector<string>{data.begin() + i * taskSize,
			data.size() < (i + 1)* taskSize ?
			data.end() : data.begin() + (i + 1) * taskSize});
		taskQueue->AddTask(task);
	}
}

DWORD WINAPI ProcessTask() {
	Task task = taskQueue->RemoveTask();
	if (!task.text.empty()) {
		sort(task.text.begin(), task.text.end());
		resultQueue->AddTask(task);
	}
	ExitThread(0);
}

vector<string> MergeSort(vector<string> v1, vector<string> v2)
{
	int i = 0, j = 0;
	vector<string> res{};
	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j])
			res.push_back(v1[i++]);
		else
			res.push_back(v2[j++]);
	}
	while (i < v1.size())
		res.push_back(v1[i++]);
	while (j < v2.size())
		res.push_back(v2[j++]);
	return res;
}