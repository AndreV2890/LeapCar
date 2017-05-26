/*
 * DiViServerSeriale.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: parallels
 */

#include "DiViClientSeriale.h"
#include "DiViSeriale.h"
#include "StateTable.h"
#include <iostream>
#include <stdlib.h>
#include <mutex>

using namespace std;

Buffer* toSend;


Signal_ST DiViClientSeriale::getSignal(){

	char temp;
	int rec_ret = DiViSeriale::receive(&temp);
    
    //If something is received it can be an ACK or not
    //if the MSB is 1 the message received is an ACK so it can be
    //the ACK related to the last message sended or not
	if(rec_ret > 0){
		unsigned char app = temp>>7;
		if(app == 0)
			printf("Primo bit: %d\n", app);
		else {
			app = temp<<4;
			if(app == 0){
				printf("Type: %d --> ACK\n", app);
				app = temp<<1;
				app = app >> 5;
				unsigned char Id = (unsigned char) IdtoACK;
				if(app == Id){
					return ack_id_correct;
				}
				else{
					return ack_id_incorrect;
				}
			}
			printf("Type: %d --> non è un ACK\n", app);
		}
	}
	
    //If we receive nothing or something that is not an ACK we have to check
    //if the timer is exipered
	if(sm.timer->active && sm.timer->count <= 0) return end_timer;
    
    //Otherwise we have to check if there is at least one message ready to be send
	if(!toSend->isEmpty()) {
		printf("messaggi da mandare!\n");
		return msg_to_send;
	}

	return signal_tick;

}

DiViClientSeriale::DiViClientSeriale(char *add,int baud):DiViSeriale(add, baud) {
	// TODO Auto-generated constructor stub
	currId = 0;

	sm.init();

	toSend = new Buffer();

}

void DiViClientSeriale::init(){
	DiViSeriale::init();
};

DiViClientSeriale::~DiViClientSeriale() {
	// TODO Auto-generated destructor stub
}

void DiViClientSeriale::tick(){

	Signal_ST sig = getSignal();

	char data[3];
	data[0] = 0xFF;
    
    //Dispatch function(Signal, char to contain the element removed from the buffer,
    //                  Pointer to the buffer, Id of the message removed form the buffer)
	sm.dispatch(sig, data, toSend, &IdtoACK);;

	if(data[0] != (char)0xFF){
		DiViSeriale::send(data);
		printf("Invio -----> \t%hhu %hhu %hhu\n",data[0],data[1],data[2]);
	}
};

void DiViClientSeriale::send_command(int vel, int rad, bool rect) {
	// built the message
	char msg[3] = {0x80,0x00,0x00};

	// new id_msg
	mutex.lock();
	currId = (currId+1)%8;
	mutex.unlock();

	// header
	msg[0] |= (currId << 4) | DATA_SEND;

	// byte speed (negative: bit 7 == 1)
	if (vel < 0) msg[1] |= 0x40;
	int vel_codif = (abs(vel)/5);
	msg[1] |= ((char) vel_codif);
	// byte rad (bit 7 == 1 if rect, otherwise bit 6 is the sign (1->negative))
	if (rect) msg[2] |= 0x40;
	else {
		if (rad < 0) msg[2] |= 0x20;
		int radius = abs(rad);
		msg[2] |= ((char) radius);
	}

	toSend->add(msg);

}
