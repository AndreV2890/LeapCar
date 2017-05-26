/*
 * Buffer.h
 *
 *  Created on: 27 apr 2016
 *      Author: andrea
 */

#ifndef SERIAL_BUFFER_H_
#define SERIAL_BUFFER_H_

#include <mutex>

#define DIM_BUFF 50
#define DIM_ELEM_BUFF 3

class Buffer {
	char buffer[DIM_BUFF][DIM_ELEM_BUFF];
	char count;
	char head;
	char tail;
	std::mutex mutex;
public:
	Buffer();
	void add(char*);
	void read(char*);
	void remove();
	bool isEmpty();
	bool isFull();
	char getCount();
};

//Buffer* toSend;

#endif /* SERIAL_BUFFER_H_ */
