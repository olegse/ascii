#include <stdio.h>
#include <stdlib.h>			/* for atoi(3) */
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#define		FILE		"/tmp/notes"

int main(int argc,char **argv) {
	int next_option;		/* option character returned by getopt_long(3) */
	int fd;						/* file descriptor */
	int offset = 0;			/* offset in bytes */
	int count;					/* count of bytes read */
	int step = 0;
	char *filename;			/* file name to read */
	char byte;
	
	filename = (char *) ec_malloc(20);

	const char *const short_options = ":f:s:";

	const struct option long_options[] = {
		{ "seek-offset",	required_argument, NULL, 's' },
		{ "file",					required_argument, NULL, 'f' },
		{ NULL,						no_argument,			 NULL,  0  }
	};

	do {
		next_option = getopt_long(argc,argv,short_options,
																				long_options,NULL);
		switch(next_option) {
			case 's':
				offset = atoi(optarg);
				break;

			case 'f':
				/* File name specified. Learn it. */
				strcpy(filename,optarg);

			case -1:
				break;

			default:
				abort();
		}
	}
	while (next_option != -1);

	
	if ( (fd = open(FILE,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) == -1) {
		perror(FILE);
		exit(EXIT_FAILURE);
	}
	
	/*** DEBUG ***/
	printf("[DEBUG] offset: %d\n",offset);
	printf("[DEBUG] fd:   %d\n",fd);

	while ( (count = read(fd,&byte,1)) > 0 ) {
		/*
		 * On error - exit.
		 */
		if (count == -1) {
			perror("Cann't read data");
			exit(EXIT_FAILURE);
		}

		/* 
		 * Print data.
		 */
		if (step >= offset) {
			if (byte == '\n')
				printf("\033[1m\\n\n\033[0m\n");
			else
				printf("\033[1m%c \033[0m",byte);
		}
		else {
			if (byte == '\n')
				printf("\033[2m\\n\n\033[0m");
			else
				printf("\033[2m%c \033[0m",byte);
		}

		step++;
	}

	/*
	 * Close file.
	 */
	if ( close(fd) == -1 ) {
		perror("Cann't close /var/notes");
		exit(EXIT_FAILURE);
	}

	return 0;
}
