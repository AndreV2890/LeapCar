/*
 * StateTable.cpp
 *
 *  Created on: 28 apr 2016
 *      Author: andrea
 */
#include "StateTable.h"
#include "Buffer.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

//To initialize the current state and the timer
void StateTable::init(){
	current_state = wait_for_msg;
	timer = new Timer();
}

//To go in the new state s
void StateTable::tran(State_ST s){
	current_state = s;
	cout << "New State: \t" << State_ST(s) << endl;
}

void StateTable::dispatch(Signal_ST s, char* temp, Buffer* toSend, char* id){
    
	StateTable::ActionState elem = myTable[current_state][s];

	switch(elem.actionToDo){
            
        //CASE remove_msg: an element of the buffer will be removed and the timer will be turned off
		case remove_msg:{
			toSend->remove();
			timer->active = false;
			break;
		}
            
        //CASE send_start: an element of the buffer will be read, the id of this message will be saved
        //                 and the timer will be turned on
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
            
        //CASE send_reset: All the elements inside the buffer with the except of the last inserted will be
        //                 removed. The last element will be read, the id of this message will be saved and the
        //                 timer will be reseted
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

