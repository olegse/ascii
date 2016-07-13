#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME		"/var/notes"

int main(int argc, char **argv) {
	int fd;
	char byte;

	if ( (fd = open(FILENAME,O_RDONLY)) == -1)
		perror("Cann't open /var/notes");

	/*
	 * Read data per byte.
	 */
	while ( read(fd,&byte,1) != 0 )
		;
	
	/*** DEBUG ***/
	printf("[DEBUG] byte:   %d\n",byte);

	if ( (close(fd)) == -1) 
		perror("Cann't open /var/notes");

	return 0;
}
