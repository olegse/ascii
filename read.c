#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "hacking.h"

#define FILENAME		"/var/notes"

char *program_name;

int main(int argc, char **argv) {
	int fd;
	int count = -1;
	char *buffer;
	char *cp;

	program_name = argv[0]+2;			/* prorgam name */

	buffer = (char *) ec_malloc(program_name,20);

	if ( (fd = open(FILENAME,O_RDONLY)) == -1)
		fatal(program_name,"Cann't open file");

	if ( (count = read(fd,buffer,srtlen(buffer))) == -1)
		fatal(program_name,"Cann't read the file");
	
	printf("Requested %d bytes.\n",strlen(buffer));
	printf("Returned %d bytes.\n",count);

	printf("[DEBUG] buffer lenght: %d\n", strlen(buffer));
	for(cp=buffer; *cp != '\0'; ++cp)
		switch(*cp) {
			case 10: printf("'\\n'\n"); break;
			default: printf("'%c' ",*cp); break;
		}

	/*** DEBUG ***/
	//printf("[DEBUG] count:   %d\n",count);

	if ( (close(fd)) == -1) 
		fatal(program_name,"Cann't close file");

	return 0;
}
