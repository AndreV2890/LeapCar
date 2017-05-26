/*
 * DiViSeriale.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: parallels
 */

#include "DiViSeriale.h"
#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

using namespace std;
DiViSeriale::DiViSeriale(char *addr, int baud) {
	strcpy(address,addr);
	baudrate = baud;
}

void DiViSeriale::init() {
	USB = open( address, O_RDWR| O_NOCTTY | O_NONBLOCK);

	if ( USB < 0 )
		cout << "Error " << errno << " opening " << address << ": " << strerror (errno) << endl;

	/* *** Configure Port *** */
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 )
		cout << "Error " << errno << " parameters not read: " << strerror(errno) << endl;

	/* Set Baud Rate */
	int code;
	switch (baudrate) {
	case 9600: code=B9600; break;
	case 19200: code=B19200; break;
	case 38400: code=B38400; break;
	case 115200: code=B115200; break;
	}

	cfsetospeed (&tty, code);
	cfsetispeed (&tty, code);

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );

	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
		cout << "Error " << errno << " new parameters not setted" << endl;

	// wait that serial ends to configurate (need some time)
	usleep(1000);
}

int DiViSeriale::send(char *cmd) {
	int n_written = write( USB, cmd, sizeof(cmd) -1);
	return n_written;
}

int DiViSeriale::receive(char* ch) {
	int n = read( USB, ch , 1 );
	if (n<1) n=0;
	return n;
}

DiViSeriale::~DiViSeriale() {
	close(USB);
}

