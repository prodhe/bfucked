// 
//  bfucked.c
//  bfucked
//  
//  Created by Petter Rodhelind on 2012-01-28.
//  Copyright 2012. All rights reserved.
// 

//
// brainfuck interpreter:
// 
// 	>		p++					move right
// 	<		p--					move left
// 	+		(*p)++				add 1
// 	-		(*p)--				subtract on
// 	.		putchar(*p)			write to stdout
// 	,		*p = getchar()		get from stdin
// 	[		while(*p) {			if *p==0, go to ]
// 	]		}					if *p!=0, go back to [
//


// IMPORT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bfucked.h"


// BRAINFUCK INTERPRETER
int brainfuck(char *sCode, size_t sizeCode, char **sOutput, int nDebug)
{	
	// initialize our brainfuck machine
	struct bfucked_t m;	// m as in machine
	
	// a zeroed tape to begin with
	int i=0;
	for (i=0; i<TAPESIZE; i++)
		m.data[i] = 0;

	// base allocate holder for final output (which will be copied into argument sOutput which will be reallocated to fit)
	m.sFinalOutput = malloc(sizeof(char)*BUFSIZE);	
	if (m.sFinalOutput != NULL)
		m.sizeStrFinalOutput = sizeof(char)*BUFSIZE;
	
	// keep track of looping
	int nLoopDepth = 0;
	int nLoopStart[256];
	int nLoopEnd[256];
	
	// placeholder for every message, before copying onto m.sFinalOutput
	char strOut[1024];
	memset(strOut, '\0', 1024);

	// loop each command
	i=0;
	while(i < strlen(sCode))
	{	
		/*
			TODO egen funktion för debug-output
			samma format för all output från kommandona?
		*/
		switch(sCode[i])
		{
			case '>':
				m.nPos++;
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d](%d)\n", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				break;
			case '<':
				m.nPos--;
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d](%d)\n", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				break;
			case '+':
				m.data[m.nPos]++;
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d] = %d\n", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				break;
			case '-':
				m.data[m.nPos]--;
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d] = %d\n", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				break;
			case '.':
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d](%d)\n", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				else
				{
					sprintf(strOut, "%c", m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				break;
			case ',':
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d](%d): ", i+1, sCode[i], m.nPos, m.data[m.nPos]);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				else
					printf(": ");
				// scanf("%c%*s\n", data[nPos]);
				m.data[m.nPos] = getchar();
				if (m.data[m.nPos] != '\n')
					getchar();	// "flush" newline
				break;
			case '[':
				// Go deeper
				nLoopDepth++;
				
				// save current position as a new loop start
				if (nDebug)
				{
					sprintf(strOut, "%d\t%c [%d](%d)\t~%d ", i+1, sCode[i], m.nPos, m.data[m.nPos], nLoopDepth);
					output(&m, strOut, (sizeof(char)*strlen(strOut)));
				}
				nLoopStart[nLoopDepth] = i;
				
				// if not null, go on
				if (m.data[m.nPos] != 0)
				{
					if (nDebug)
					{
						sprintf(strOut, "enter\n");//, nLoopDepth);
						output(&m, strOut, (sizeof(char)*strlen(strOut)));
					}
				}
				// else, jump to ending tag
				else
				{					
					// skip forward
					if (nDebug)
					{
						sprintf(strOut, "forward\n");//, i+1, nPos, data[nPos], nLoopDepth);
						output(&m, strOut, (sizeof(char)*strlen(strOut)));
					}
					while ((sCode[i] != ']'))// || (sCode[i] != '[') || (sCode[i] != '\0'))
					{
						if (nDebug && sCode[i+1]!=']')
						{
							sprintf(strOut, "%d\t skipping %c\n", i+1, sCode[i+1]);
							output(&m, strOut, (sizeof(char)*strlen(strOut)));
						}
						i++;
					}

					// loop's done - what stopped us? A new depth?
					if (sCode[i] == '[')
					{
						// back one and return to main loop to do all this again
						i--;
					}
					// Seems like it was the end for this depth...
					else if (sCode[i] == ']')
					{
						// save endloop for this depth, back one up and return to main loop
						nLoopEnd[nLoopDepth] = i;
						i--;
					}
				}
				break;
			case ']':
				// if positive, return to start pos and stay in this depth
				if (m.data[m.nPos])
				{
					if (nDebug)
					{
						sprintf(strOut, "%d\t%c [%d](%d)\t~%d rewind\n\t[ [%d](%d)\t~%d enter\n", i+1, sCode[i], m.nPos, m.data[m.nPos], nLoopDepth, m.nPos, m.data[m.nPos], nLoopDepth);
						output(&m, strOut, (sizeof(char)*strlen(strOut)));
					}
					i = nLoopStart[nLoopDepth];
				}
				// the loop is finished (position=null), so leave the current depth and just go on
				else
				{
					if (nDebug)
					{
						sprintf(strOut, "%d\t%c [%d](%d)\t~%d stop\n", i+1, sCode[i], m.nPos, m.data[m.nPos], nLoopDepth);
						output(&m, strOut, (sizeof(char)*strlen(strOut)));
					}
					nLoopDepth--;
				}
				break;
		}
		
		// move to next character
		i++;
	}
	
	// copy our final output into given string
	char *tmp = realloc(*sOutput, m.sizeStrFinalOutput);
	if (tmp != NULL)
	{
		// printf("realloc ok\n");
		*sOutput = tmp;
		strncpy(*sOutput, m.sFinalOutput, m.sizeStrFinalOutput);
		tmp = NULL;
	}
	
	// clean up and return
	free(m.sFinalOutput);
	return 0;
}

// OUTPUT TO SPECIFIED FILE DESCRIPTOR
int output(struct bfucked_t *m, char *str, size_t sizeStr)
{
	if(strlen(m->sFinalOutput) > m->sizeStrFinalOutput-100)
	{
		// printf("%p: doubleup %lu\n", m->sFinalOutput, strlen(m->sFinalOutput));
		char *tmp = realloc(m->sFinalOutput, m->sizeStrFinalOutput*2);
		if (tmp != NULL)
		{
			m->sFinalOutput = tmp;
			m->sizeStrFinalOutput = m->sizeStrFinalOutput * 2;
		}
	}

	strncat(m->sFinalOutput, str, sizeStr);
	
	return 0;
}
	