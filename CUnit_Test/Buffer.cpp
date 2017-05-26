/*
 * Buffer.cpp
 *
 *  Created on: 27 apr 2016
 *      Author: andrea
 */

#include "Buffer.h"

Buffer::Buffer() {
	count = 0;
	head = 0;
	tail = 0;
}
void Buffer::add(char* msg) {
	mutex.lock();
	int i;
	for (i = 0; i < 3; i++)	buffer[tail][i] = msg[i];
	tail = (tail+1)%DIM_BUFF;
	count++;
	mutex.unlock();
}
void Buffer::read(char* msg) {
	mutex.lock();
	int i;
	for (i = 0; i < 3; i++) msg[i] = buffer[head][i];
	mutex.unlock();
}

void Buffer::remove() {
	mutex.lock();
	head = (head+1)%DIM_BUFF;
	count--;
	mutex.unlock();
}
bool Buffer::isEmpty() {
	bool ret;
	mutex.lock();
	ret = (count == 0);
	mutex.unlock();
	return ret;
}
bool Buffer::isFull() {
	bool ret;
	mutex.lock();
	ret = (count == DIM_BUFF);
	mutex.unlock();
	return ret;
}

char Buffer::getCount() {
	char temp;
	mutex.lock();
	temp = count;
	mutex.unlock();
	return temp;
}



