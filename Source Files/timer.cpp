#include "timer.h"
#include <SDL3/SDL.h>

Timer::Timer()
{
	start_time = 0;
	stop_time = 0;
	is_running = false;
	is_stopped = false;
}

void Timer::start()
{
	start_time = SDL_GetTicks();
	is_running = true;
}

void Timer::reset()
{
	start_time = 0;
	stop_time = 0;
	is_running = false;
	is_stopped = false;
}

void Timer::stop()
{
	is_running = false;
	is_stopped = true;
	start_time = 0;
}

float Timer::getTime()
{
	return SDL_GetTicks() - start_time;
}
