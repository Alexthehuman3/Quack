#pragma once
#include <chrono>
struct DoomTime
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::chrono::duration<double> runtime = end - start;

	double durationSecond = runtime.count();
};

