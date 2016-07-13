#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *program_name;
void mode_check(const char *mode) {
	printf("strlen(mode): %d\n", strlen(mode));
	if ( strlen(mode) > 3) {
		fprintf(stderr,"%s: File creation mask to long.\n",program_name);
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char **argv) {
	program_name = argv[0]+2;
	char mode[10] = {0};

	strcpy(mode,"0744");
	mode_check(mode);
	return 0;
}
