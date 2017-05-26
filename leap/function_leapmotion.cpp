#include <pthread.h>
#include <iostream>

#include "function_leapmotion.h"
#include "MyCarMotion.h"
#include "../threads_managing/my_thread_lib.h"

using namespace Leap;

MyCarMotion* my_car;

void* thread_hand(void* arg){
	TaskPar *tp;
	tp = (TaskPar *) arg;

	DiViClientSeriale* client;
	client = (DiViClientSeriale*) (tp->arg);

	my_car = new MyCarMotion(client);

	set_period(tp);

	while(1){
		my_car->dispatch();
		wait_for_period(tp);
	}
}

void* thread_tick(void*arg){
	TaskPar *tp;
	tp = (TaskPar *) arg;
	DiViClientSeriale* client;
	client = (DiViClientSeriale*) (tp->arg);

	set_period(tp);
	while(1){
		client->tick();
		wait_for_period(tp);
		client->sm.timer->count-=tp->period;
	}
}
