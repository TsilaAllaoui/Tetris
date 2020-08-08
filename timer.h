#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>

class Timer
{
	private:
	    float start_time, stop_time;
	    bool is_running, is_stopped;
	public:
	    Timer();
	    void start();
	    void stop();
	    void reset(float t);
	    float get_time();
};


#endif
