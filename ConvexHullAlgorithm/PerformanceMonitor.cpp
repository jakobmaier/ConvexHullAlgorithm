#include "PerformanceMonitor.h"

#include <math.h>



PerformanceMonitor::PerformanceMonitor() {
	QueryPerformanceFrequency(&mFrequency);
}

void PerformanceMonitor::start() {
	QueryPerformanceCounter(&mStartTicks);
}

double PerformanceMonitor::stop() {
	QueryPerformanceCounter(&mStopTicks);
	mMillis = -(mStartTicks.QuadPart - mStopTicks.QuadPart) * 1000.0 / mFrequency.QuadPart;;
	return mMillis;
}


double PerformanceMonitor::millis() const {
	return mMillis;
}

std::string PerformanceMonitor::millisToString(double millis) {
	long iMillis = static_cast<long>(floor(millis));
	long sek = iMillis / 1000;
	long min = sek / 60;
	long h = min / 60;

	char str[12 + 1];
	sprintf(str, "%02d:%02d:%02d.%03d", h % 100, min % 60, sek % 60, iMillis % 1000);
	return std::string(str);
}

std::string PerformanceMonitor::toString() const {	//hh:mm:ss.mmm
	return PerformanceMonitor::millisToString(mMillis);
}