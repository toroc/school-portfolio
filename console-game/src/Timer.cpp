#include "../inc/Timer.hpp"
using namespace std;

/*Source Code from http://stackoverflow.com/questions/21521282/basic-timer-with-stdthread-and-stdchrono */

/*Made changes to code so that it would work*/
Timer::Timer()
{
	running = true;
}
void Timer::start(const Interval &interval, const Timeout &timeout)
{
	running = true;
    th = thread (runTimer, interval, timeout, running);
}
void Timer::stop()
{ 
	running = false;
	th.join();
}


