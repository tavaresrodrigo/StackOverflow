#include <signal.h>
#include <stdio.h>
#include <string.h>
int main(){
	char realPassword[8];
	char givenPassword[8];

	strncpy(realPassword, "ffffffff", 8);
	gets(givenPassword);
	
	if (0 == strncmp(givenPassword, realPassword, 8)){
		printf("SUCCESS!\n");
	}else{
		printf("FAILURE!\n");
	}
	raise(SIGINT);
	printf("givenPassword: %s\n", givenPassword);
    printf("realPassword: %s\n", realPassword);
	return 0;
}