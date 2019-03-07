//William Tai
//CS240, Spring 2019
//HW#2
//Framework for a shell that uses
//bash syntax for commands

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>	//set for exec to indicate error if = -1

//function prototypes
void GetCMD();		//get user command
void ParseCMD(char *LineBuff);		//parses user command into usable commands
void Spawn(char *CMDArray[]); 

int main()
{
    GetCMD();

    return 0;
}

void GetCMD()
{
    char Character;
    int i;
    int Repeat = 1;

    do
    {
	char LineBuffer[256] = {"\0"};	//clears the buffer at the beg of instance
	printf("\n?: ");		//shell prompt
	i = 0;				//resets i counter
	
	while ((Character = getchar()) != 0xA)	//detects carriage return
	{
	    LineBuffer[i] = Character;

	    if (strcmp(LineBuffer, "exit") == 0) //detects exit call
	        exit(1);
	    else
		i++;
	}

	ParseCMD(LineBuffer);		//passes to parser
    }while(Repeat);			//repeat until exit
}

void ParseCMD(char *LineBuff)
{
    char *TVector;		
    char *SpaceChar = " ";
    char **Line = malloc(256 * sizeof(char *));
    int i = 0;				//counter

    TVector = strtok(LineBuff, SpaceChar);	//parses for first command vectr
    //printf("test1\n");
    
    while(TVector != NULL)
    {
        Line[i] = TVector;//similar to argv
        //printf("test2\n");
	TVector = strtok(NULL, SpaceChar);	//vector reset
	printf("%s\n", Line[i]);
	i++;
    }
    //printf("test2.5\n");
    Line[i] = NULL;
    //printf("test3\n");
    Spawn(Line);
}

void Spawn(char **CMDArray)
{
    pid_t Value;
    int status;

    Value = fork();

    if (Value == -1)	//fork error
    {
	perror("Error: Failed to Fork!");
	exit(-1);
    }
    else if (Value == 0)	//child code
    {
	if (execvp(CMDArray[0], CMDArray) < 0)	//will return -1 if error
	{
	    perror("Error: %s");
	}

	exit(0);
    }
    else		//parent code
    {
	Value = wait(&status);	//wait for child exit
    }
}
