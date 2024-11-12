#pragma once
#include "EngineHeader.h"

namespace ThreadUtil {
	void New(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param=NULL);
	bool IsRunning(HANDLE& HandleValue);
	void Delete(HANDLE& HandleValue);
	void InitSection(CRITICAL_SECTION& Section);
	void InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount);
	void DeleteSection(CRITICAL_SECTION& Section);
	void Lock(CRITICAL_SECTION& Section);
	void TryLock(CRITICAL_SECTION& Section);
	void Unlock(CRITICAL_SECTION& Section);
}