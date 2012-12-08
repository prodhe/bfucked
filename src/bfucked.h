// 
//  bfucked.h
//  bfucked
//  
//  Created by Petter Rodhelind on 2012-01-28.
//  Copyright 2012. All rights reserved.
// 

#define BUFSIZE		1024
#define TAPESIZE	30000	// standard bf implementation

// OUR MACHINE
struct bfucked_t
{
	// tape
	char data[TAPESIZE];
	
	// pointer/reader
	int nPos;
	
	// logging and output
	char *sFinalOutput;		// remember to (re)allocate sufficient space
	size_t sizeStrFinalOutput;
};


int brainfuck(char* sCode, size_t sizeCode, char **sOutput, int nDebug);
int output(struct bfucked_t *m, char *str, size_t sizeStr);