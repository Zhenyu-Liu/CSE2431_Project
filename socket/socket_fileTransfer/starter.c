#include <stdlib.h>

void server(){
	system("./server &");
	system("disown");
}

int main(){
	server();
}
