/*
 * DiViServerSeriale.h
 *
 *  Created on: Feb 19, 2016
 *      Author: parallels
 */

#ifndef DIVICLIENTSERIALE_H_
#define DIVICLIENTSERIALE_H_

#include "DiViSeriale.h"
#include "StateTable.h"
#include <mutex>

// type message
#define DATA_SEND 0x01
#define ACK 0x00

class DiViClientSeriale: public DiViSeriale {
private:
	char currId;
	std::mutex mutex; // to protect currId

	char IdtoACK;

public:
	DiViClientSeriale(char *,int);

	void init();
	// blocking calling
	void tick();

	Signal_ST getSignal();
	void send_command(int vel, int rad, bool rect);
	int request_data();
	virtual ~DiViClientSeriale();

	//STATE MACHINE
	StateTable sm;
    
};

#endif /* DIVISERVERSERIALE_H_ */
