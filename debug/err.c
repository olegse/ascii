/* 
 * Functions source code. Requires headers.h to be included.
 */
#include <stdio.h>			/* for perror(3) */
#include <stdlib.h>			/* for exit(3) */
#include "err.h"

/* Print error meessage prefixed by prorgam name and exit */
void fatal(char *msg) {
	fprintf(stderr,"%s : ", program_name);
	perror(msg);
	exit(EXIT_FAILURE);
}

/* 
 * ec_malloc():		performs error-checked call to malloc(3)
 */
void *ec_malloc(unsigned int s) {
	void *p;
	if( (p = malloc(s) ) == NULL ) {
		fatal("Cann't allocate memory");
		exit(EXIT_FAILURE);
	}
	return p;
}
