/*
 * StateTable.h
 *
 *  Created on: 28 apr 2016
 *      Author: andrea
 */

#ifndef SERIAL_STATETABLE_H_
#define SERIAL_STATETABLE_H_

#include "Timer.h"
#include "Buffer.h"

enum State_ST {wait_for_ACK, wait_for_msg, null, MAX_STATE};
enum Signal_ST {ack_id_correct, ack_id_incorrect, msg_to_send, end_timer, signal_tick, MAX_SIGNAL};
enum action_ST {do_nothing, remove_msg, send_start, send_reset};

typedef struct{
	action_ST actionToDo;
	State_ST nextState;
}ActionState;

class StateTable{

public:

	State_ST current_state;

	ActionState myTable[MAX_STATE][MAX_SIGNAL] = {
		{ {remove_msg,wait_for_msg}, {do_nothing,wait_for_ACK}, {do_nothing,wait_for_ACK}, {send_reset, wait_for_ACK}, {do_nothing,wait_for_ACK} },
		{ {do_nothing,wait_for_msg}, {do_nothing,wait_for_msg}, {send_start,wait_for_ACK}, {do_nothing, wait_for_msg}, {do_nothing,wait_for_msg} },
	};

	Timer* timer;

	void init();
	void dispatch(Signal_ST, char*, Buffer*, char*);
	action_ST dispatch1(State_ST, Signal_ST);
	State_ST dispatch2(State_ST, Signal_ST);
	void tran(State_ST);

};

#endif /* SERIAL_STATETABLE_H_ */
