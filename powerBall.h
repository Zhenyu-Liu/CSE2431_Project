#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Generate random number from 1 to 30
int randomNumber(){
	srand((unsigned)time(NULL));   
    int result=rand()%30+1;
    return result;
}

int PBmain(){
	printf("Let's play power ball!\n");
	int a=randomNumber();
	
	
	
	printf("Please input your three numbers (from 1 to 30):\n");
	int inA,inB,inC;
	scanf("%d",&inA);
	scanf("%d",&inB);
	int b=randomNumber();
	scanf("%d",&inC);
	int count=0;

	if(a==inA)
		count++;
	if(b==inB)
		count++;
	int c=randomNumber();
	if(c==inC)
		count++;
	if(count==1||count==2){
		printf("1 or 2 match, some malware will pause this time!\n");
	}
	else if(count==3){
		printf("All match! the malware will shut down!\n");
	}
	else{
		printf("No one matches.\n");
	}
	return count;
}
