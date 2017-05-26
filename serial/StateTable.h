/*
 * StateTable.h
 *
 *  Created on: 28 apr 2016
 *      Author: andrea
 */

#ifndef SERIAL_STATETABLE_H_
#define SERIAL_STATETABLE_H_

#include "Buffer.h"
#include "Timer.h"

enum State_ST {wait_for_ACK, wait_for_msg, null, MAX_STATE};
enum Signal_ST {ack_id_correct, ack_id_incorrect, msg_to_send, end_timer, signal_tick, MAX_SIGNAL};
enum action_ST {do_nothing, remove_msg, send_start, send_reset};

class StateTable{

    public:
    
        //The type ActiveState represents the pair actionToDo and nextState contained inside each entry of the
        //State Table
        typedef struct{
            action_ST actionToDo;
            State_ST nextState;
        }ActionState;
    
        //To represent the current state
        State_ST current_state;

        //State Table
        ActionState myTable[MAX_STATE][MAX_SIGNAL] = {
            { {action_ST::remove_msg,State_ST::wait_for_msg}, {action_ST::do_nothing,State_ST::wait_for_ACK}, {action_ST::do_nothing,State_ST::wait_for_ACK}, {action_ST::send_reset, State_ST::wait_for_ACK}, {action_ST::do_nothing,State_ST::wait_for_ACK} },
            { {action_ST::do_nothing,State_ST::wait_for_msg}, {action_ST::do_nothing,State_ST::wait_for_msg}, {action_ST::send_start,State_ST::wait_for_ACK}, {action_ST::do_nothing, State_ST::wait_for_msg}, {action_ST::do_nothing,State_ST::wait_for_msg} },
        };

        //Pointer to timer object to manage the time (useful for checking signals)
        Timer* timer;

        //Functions for the State Machine
        void init();
        void dispatch(Signal_ST, char*, Buffer*, char*);
        void tran(State_ST);

};

#endif /* SERIAL_STATETABLE_H_ */
