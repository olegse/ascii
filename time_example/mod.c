#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	int i;

	for(i=0; i%16 != 15; i++)
		;
	printf("i:   %d\n",i);
	return 0;
}
