#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#include <iostream>

#include "threads_managing/my_thread_lib.h"
#include "leap/function_leapmotion.h"
#include "serial/DiViClientSeriale.h"

using namespace std;

int main(int argc, char** argv){

	pthread_t t_hand, t_tick;
	TaskPar t_hand_par, t_tick_par;

	char address[] = "/dev/ttyUSB0";
	DiViClientSeriale client(address, 9600);
	client.init();
	sleep(1);

	cout << "Attesa del reset della macchina" << endl;

	bool test = false;
	char tmp;

	while(!test){
		if(client.receive(&tmp) > 0){
			if(tmp == 'a'){
				test = true;
			}
		}
	}

	cout << "Avvio Programma" << endl;

	set_taskpar(&t_hand_par, &client, NULL, 50, 50, 21);
	set_taskpar(&t_tick_par, &client, NULL, 15, 15, 21);

	pthread_create(&t_hand, NULL, thread_hand, &t_hand_par);
	pthread_create(&t_tick, NULL, thread_tick, &t_tick_par);

	pthread_join(t_hand,NULL);
	pthread_join(t_tick,NULL);

	return 0;
}
