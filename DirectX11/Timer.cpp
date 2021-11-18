#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept
{
	mLast = steady_clock::now();
}

float Timer::Mark() noexcept
{
	const auto old = mLast;
	mLast = steady_clock::now();
	const duration<float> frameTime = mLast - old;
	return frameTime.count();
}

float Timer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - mLast).count();
}
