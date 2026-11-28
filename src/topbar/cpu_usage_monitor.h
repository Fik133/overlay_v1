#pragma once
#include <Windows.h>
class CPUUsageMonitor {
	ULARGE_INTEGER lastIdleTime = {};
	ULARGE_INTEGER lastKernelTime = {};
	ULARGE_INTEGER lastUserTime = {};

	ULONGLONG SubtractTimes(ULARGE_INTEGER a, ULARGE_INTEGER b) {
		return (a.QuadPart - b.QuadPart);
	}

public:
	double GetCPUUsagePercent() {
		FILETIME idleTime, kernelTime, userTime;
		if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
			return 0.0;
		}

		ULARGE_INTEGER idle, kernel, user;
		idle.LowPart = idleTime.dwLowDateTime;
		idle.HighPart = idleTime.dwHighDateTime;
		kernel.LowPart = kernelTime.dwLowDateTime;
		kernel.HighPart = kernelTime.dwHighDateTime;
		user.LowPart = userTime.dwLowDateTime;
		user.HighPart = userTime.dwHighDateTime;

		ULONGLONG idleDiff = SubtractTimes(idle, lastIdleTime);
		ULONGLONG kernelDiff = SubtractTimes(kernel, lastKernelTime);
		ULONGLONG userDiff = SubtractTimes(user, lastUserTime);

		lastIdleTime = idle;
		lastKernelTime = kernel;
		lastUserTime = user;

		ULONGLONG totalSys = kernelDiff + userDiff;
		if (totalSys == 0) return 0.0;

		double cpuUsage = static_cast<double>(totalSys - idleDiff) / static_cast<double>(totalSys) * 100.0;
		return cpuUsage;
	}
};

struct MemoryInfo {
	double totalGB;
	double usedGB;
	unsigned long long memoryLoadPercent;
};

inline MemoryInfo GetSystemMemoryUsage() {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	double total = static_cast<double>(statex.ullTotalPhys);
	double available = static_cast<double>(statex.ullAvailPhys);
	double used = total - available;

	const double gbDiv = 1024.0 * 1024.0 * 1024.0;

	return {
		total / gbDiv,
		used / gbDiv,
		statex.dwMemoryLoad 
	};
}