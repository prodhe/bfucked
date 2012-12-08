// 
//  main.c
//  bfucked
//  
//  Created by Petter Rodhelind on 2012-01-28.
//  Copyright 2012. All rights reserved.
// 



// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "bfucked.h"


// MAIN FUNCTION
int main(int argc, char *argv[])
{
	// main vars
	int nArgDebug=0;
	int nArgStdin=0;
	
	char sBuf[BUFSIZE];
	memset(sBuf, '\0', BUFSIZE);
	
	char sFileName[250];
	
	int fdInput, fdFile;
	char *sInputData;	
	sInputData = calloc(BUFSIZE, sizeof(char));
	if (sInputData == NULL)
		exit(1);
	memset(sInputData, '\0', BUFSIZE); // avoid empty memory in function call later on
	
	// loop args and check for valid options
	if (argc > 1)
	{
		// keep count of looped arguments;
		int nArgsLeft=argc-1;

		int i;
		for (i=0; i<argc; i++)
		{
			// -d	debug
			if (!strcmp(argv[i], "-d"))
			{
				nArgDebug = 1;
				nArgsLeft--;
			}
		}
		
		// if arguments left, assume that the last argument is a file name
		if (nArgsLeft == 1)
		{
			strcpy(sFileName, argv[argc-1]);
		}
		// else we read from stdin
		else
			nArgStdin=1;
	}
	// if no arguments, assume stdin
	else
		nArgStdin=1;
	
	// header
	// if (nArgDebug)
	// 	printf("Brainfuck Interpreter\n\n");
	
	// INPUT DATA
	if (nArgStdin)
	{		
		// copy current stdin file desc to fdInput
		fdInput = dup(STDIN_FILENO);
		
		/*
			TODO check if stdin is empty and abort with error message
		*/
		
	}
	else
	{
		// read file name from first argument (which is now stored in sFileName)
		fdFile=open(sFileName, O_RDONLY);
		if(fdFile < 1)
		{
			perror("Could not access file");
			return 1;			
		}
		// file success	
		else
		{
			// copy file descriptor to fdInput
			fdInput = dup(fdFile);
			if (fdInput < 1)
			{
				perror("Could not dup() on fdFile to fdInput");
				return 1;
			}
			
			close(fdFile);	
		}
	}
	
	// READ INPUT (from fdInput which is either stdin or file from above)
	int nCount=0;
	while ((nCount=read(fdInput, sBuf, BUFSIZE)))
	{
		strcat(sInputData, sBuf);
		memset(sBuf, '\0', BUFSIZE);
	}
	close(fdInput);

	// DEBUG
	if (nArgDebug)
	{
		printf("\nCODE:\n%s\n", sInputData);
		printf("___________________________________________________\n\n");
	}
	
	// pointer to save return output
	char *sOutput = NULL;
	
	// EXECUTE CODE
	int ret;
	ret=brainfuck(sInputData, (sizeof(char)*strlen(sInputData)), &sOutput, nArgDebug);
	
	// output
	if (nArgDebug)
	{
		printf("%s\n___________________________________________________\n\nEOF\n\n", sOutput);
	}
	else
	{
		printf("%s\n", sOutput);
	}
	
	if (sOutput == NULL)
	{
		printf("No return output from brainfuck code.\n");
	}
	
	// clean up
	free(sOutput);
	free(sInputData);
	
	return 0;
}