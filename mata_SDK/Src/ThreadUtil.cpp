#include "ThreadUtil.h"

ThreadUtil threadUtil;

void ThreadUtil::Create(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param) {
	HandleValue = CreateThread(NULL, 0, ThreadFunction, Param, 0, NULL);
}

bool ThreadUtil::CheckAlive(HANDLE& HandleValue) {
	DWORD Result;
	GetExitCodeThread(HandleValue, &Result);

	if (Result == STILL_ACTIVE)
		return true;

	return false;
}

void ThreadUtil::Close(HANDLE& HandleValue) {
	if(HandleValue)
		CloseHandle(HandleValue);
}

void ThreadUtil::InitSection(CRITICAL_SECTION& Section) {
	InitializeCriticalSection(&Section);
}

bool ThreadUtil::InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount) {
	return InitializeCriticalSectionAndSpinCount(&Section, SpinCount);
}

void ThreadUtil::DeleteSection(CRITICAL_SECTION& Section) {
	DeleteCriticalSection(&Section);
}

void ThreadUtil::Lock(CRITICAL_SECTION& Section) {
	EnterCriticalSection(&Section);
}

void ThreadUtil::TryLock(CRITICAL_SECTION& Section) {
	TryEnterCriticalSection(&Section);
}

void ThreadUtil::Unlock(CRITICAL_SECTION& Section) {
	LeaveCriticalSection(&Section);
}