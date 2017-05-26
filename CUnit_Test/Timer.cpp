/*
 * Timer.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: andrea
 */
#include "Timer.h"
#include <stdlib.h>

using namespace std;

Timer::Timer(){
	count = TIMER;
	active = false;
}

void Timer::active_timer(){
	active = true;
}

void Timer::reset(){
	count = TIMER;
}



