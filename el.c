#include <stdio.h>
#include <string.h>

int main(int argc,char **argv) {
	printf("%d\n",strncmp((char *)argv[1],"--ooaa",2));
	return 0;
}
