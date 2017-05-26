/*
 * Timer.h
 *
 *  Created on: 29 apr 2016
 *      Author: andrea
 */

#ifndef SERIAL_TIMER_H_
#define SERIAL_TIMER_H_

#define TIMER 100

//Timer is a simple class where the count variable is the amount of time remaining before the timer expires.
//This count will be decreased each time of a time equal to the thread tick period

class Timer{
	public:
		int count;
		bool active;
		Timer();
		void reset();
		void active_timer();
};

#endif /* SERIAL_TIMER_H_ */
