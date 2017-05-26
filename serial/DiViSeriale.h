/*
 * DiViSeriale.h
 *
 *  Created on: Feb 19, 2016
 *      Author: parallels
 */

#ifndef DIVISERIALE_H_
#define DIVISERIALE_H_

#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

class DiViSeriale {
private:
	char address[13];
	int USB;			// connection
	int baudrate;		// baudrate of USB
	struct termios tty;	// parameters of USB connection
public:
	DiViSeriale(char *,int);
	void init();
	virtual ~DiViSeriale();
	int send(char *);
	int receive(char *);
};

#endif /* DIVISERIALE_H_ */
