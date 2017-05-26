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

//To add a messege inside the buffer
void Buffer::add(char* msg) {
	mutex.lock();
	int i;
	for (i = 0; i < 3; i++)	buffer[tail][i] = msg[i];
	tail = (tail+1)%DIM_BUFF;
	count++;
	mutex.unlock();
}

//To read the first (oldest message) message from the buffer
void Buffer::read(char* msg) {
	mutex.lock();
	int i;
	for (i = 0; i < 3; i++) msg[i] = buffer[head][i];
	mutex.unlock();
}

//To remove the oldest message from the buffer
void Buffer::remove() {
	mutex.lock();
	head = (head+1)%DIM_BUFF;
	count--;
	mutex.unlock();
}

//To check if the buffer is empty
bool Buffer::isEmpty() {
	bool ret;
	mutex.lock();
	ret = (count == 0);
	mutex.unlock();
	return ret;
}

//To check if the buffer is full
bool Buffer::isFull() {
	bool ret;
	mutex.lock();
	ret = (count == DIM_BUFF);
	mutex.unlock();
	return ret;
}

//To return  the number of messages inside the buffer
char Buffer::getCount() {
	char temp;
	mutex.lock();
	temp = count;
	mutex.unlock();
	return temp;
}



