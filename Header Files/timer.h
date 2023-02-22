#pragma once

// Timer class
class Timer
{
	private:
		// Start and Stop time
	    float start_time, stop_time;

		// Timer state
	    bool is_running, is_stopped;
	
	public:
		// Constructor and Destructor
	    Timer();

		/**
		*\ brief Start the timer
		*\ return
		*/
	    void start();
	    
		/**
		*\ brief Stop the timer
		*\ return
		*/
		void stop();

		/**
		*\ brief Reset the timer
		*\ return
		*/
	    void reset();

		/**
		*\ brief Get the timer elapsed current time
		*\ return the current time elapsed
		*/
	    float getTime();
};
