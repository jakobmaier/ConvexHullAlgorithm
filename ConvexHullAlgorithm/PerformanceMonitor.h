#pragma once


#define  _CRT_SECURE_NO_WARNINGS true

#include <string>
#include <windows.h>
#include "ctime"

class PerformanceMonitor {
	LARGE_INTEGER mFrequency;        // ticks per second
	LARGE_INTEGER mStartTicks;
	LARGE_INTEGER mStopTicks;
	double mMillis;
public:
	PerformanceMonitor();
	void start();
	double stop();

	double millis() const;
	static std::string millisToString(double millis); //hh:mm:ss.mmm
	std::string toString() const; //hh:mm:ss.mmm
};

