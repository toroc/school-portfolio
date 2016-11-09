#define _GLIBCXX_USE_NANOSLEEP
#ifndef TIMER_HPP
#define TIMER_HPP
#include <iostream>
#include <thread>
#include <chrono>

/*Source Code from http://stackoverflow.com/questions/21521282/basic-timer-with-stdthread-and-stdchrono */

using namespace std;

class Timer
{
protected:
	thread th;
	bool running;

public:
	Timer();
	typedef std::chrono::milliseconds Interval;
	typedef std::function<void(void)> Timeout;

	void start(const Interval &interval, const Timeout &timeout);
	void stop();

	void static runTimer(Timer::Interval &interval, Timer::Timeout &timeout, bool &running)
	{
		while (running == true)
		{
			this_thread::sleep_for(interval);
			timeout();
		}
	}
};
#endif