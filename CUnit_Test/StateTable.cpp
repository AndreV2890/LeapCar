/*
 * StateTable.cpp
 *
 *  Created on: 28 apr 2016
 *      Author: andrea
 */
#include "StateTable.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

void StateTable::init(){
	current_state = wait_for_msg;
	timer = new Timer();
}

void StateTable::tran(State_ST s){
	current_state = s;;
}

action_ST StateTable::dispatch1(State_ST current, Signal_ST s){
	ActionState elem = myTable[current][s];

	if(elem.nextState!= null && elem.nextState != current)
		tran(elem.nextState);

	return elem.actionToDo;
}

State_ST StateTable::dispatch2(State_ST current, Signal_ST s){
	ActionState elem = myTable[current][s];

	if(elem.nextState!= null && elem.nextState != current)
		tran(elem.nextState);

	return elem.nextState;
}

void StateTable::dispatch(Signal_ST s, char* temp, Buffer* toSend, char* id){
	ActionState elem = myTable[current_state][s];

	switch(elem.actionToDo){
		case remove_msg:{
			toSend->remove();

			timer->active = false;
			break;
		}
		case send_start:{
			toSend->read(temp);
			unsigned char utemp = (unsigned char) *temp;
			unsigned char temp_id;
			temp_id = utemp << 1;
			temp_id = temp_id >> 5;
			*id = (char)temp_id;

			timer->active_timer();
			timer->reset();
			break;
		}
		case send_reset:{
			while(toSend->getCount() > 1)
				toSend->remove();
			toSend->read(temp);
			unsigned char utemp = (unsigned char) *temp;
			unsigned char temp_id;
			temp_id = utemp << 1;
			temp_id = temp_id >> 5;
			*id = (char)temp_id;

			timer->reset();
			break;
		}
		case do_nothing: break;
	}

	if(elem.nextState!= null && elem.nextState != current_state)
		tran(elem.nextState);
}

