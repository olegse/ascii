/*
 * miss			return 1 if file exist othewise 0
 */
#include <stdio.h>
#include <unistd.h>
#include "err.h"

char *program_name;

int main(int argc,char **argv) {
	int i;

	program_name = argv[0]+2;
	for(i=1; i < argc; ++i)
		printf("%s :\t%d\n",argv[i],access(argv[i],F_OK));
	
	printf("F_OK:   %d\n",F_OK);
	return 0;
}
