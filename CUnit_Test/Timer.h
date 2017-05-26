/*
 * Timer.h
 *
 *  Created on: 29 apr 2016
 *      Author: andrea
 */

#ifndef SERIAL_TIMER_H_
#define SERIAL_TIMER_H_

#define TIMER 100

class Timer{
	public:
		int count;
		bool active;
		Timer();
		void reset();
		void active_timer();
};

#endif /* SERIAL_TIMER_H_ */
