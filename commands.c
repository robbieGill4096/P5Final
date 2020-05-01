#include<stdio.h>
#include<string.h>
#include <errno.h>
#include <unistd.h>
#include<stdlib.h>
#include "smash.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void init_history(void); //builds data structures for recording cmd history
void add_history(char *cmd, int exitStatus); //Adds an entry to the history
void clear_history(void); //Frees all malloc'd memory in the history
void print_history(int firstSequenceNumber); //Prints the history to stdout

void executeExternalCommand(char **str, char *target);
void executeCommand(char *str);
char *target;

void executeExternalCommand(char **str, char *target) {
     pid_t pid;
     int status;
	
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
		
		int exitStatus = execvp(str[0], str);
		if(exitStatus == -1)
		{	int index=0;
			while(str[index] != NULL)
			{
			
			printf("%s ",str[index]);	
			index++;	
			}
			printf(": command not found\n");
		}
		
		exit(1);
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
		;
     }
	if(status == 0){
	add_history(target,0);}
	else
	{
	add_history(target,1);		
	}
}

void executeCommand(char *str)
{	
	
	char current_dir_path[100]; 
	char *args[1000];
	int i =0;

	char *target=malloc(4096);
	memcpy(target,str,4096);//removed * from *memcpy
	
	char *p = strtok (str," ");
	while (p != NULL)
	{
		args[i++] = p;
		p = strtok (NULL, " "); 
	}
	int command_count =i;
//--------------------------------------------------------------
//--------------------------------------------------------------
	//printf("the value of str is:%s\n",target);

	if (strcmp(args[0], "exit") == 0) 
	{
	  clear_history();
	  free(target);
	  exit(0);
	} 
	else if (strcmp(args[0], "cd") == 0)
	{
		if(command_count >=2) // makes sure cd also contains a path name.
		{
			if(chdir(args[1]) == 0)//checks that directory user attempted to cd to exists 0 ==true
			{
				add_history(target,0);
				printf("%s\n",getcwd(current_dir_path, 256)); 
					
			}
			else //if directory is not found prints error message including failed directory name. 
			{
				printf("%s: No such file or directory\n",target);
				add_history(target,1);
			}
		}
	}
	else if (strcmp(args[0], "history") == 0)
	{
		if(command_count == 1 ||atoi(args[1]) == 0 )
		{
			add_history(target,0);
			print_history(0);
		}
	}
	else
	{	
		char *strict_args[command_count+1];
		for (int index=0; index < command_count;index++)
		{
		strict_args[index] = args[index];
		}
		strict_args[command_count] = NULL;
		executeExternalCommand(strict_args,target);

		
	}

	free(target);
}



