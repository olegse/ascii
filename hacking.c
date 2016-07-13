#include <stdio.h>
#include "hacking.h"

int main(int argc,char **argv) {
	const char *program_name=argv[0]+2;
	fatal(program_name,"malloc");

	return 0;
}
