/*
 * lseek.c		testing program for offset.c
 */
#include <stdio.h>
#include <stdlib.h>			/* for atoi(3) */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#define		FILE		"/var/notes"

void fatal(char *);
void *ec_malloc(unsigned int);

const char *program_name;

int main(int argc,char **argv) {
	int next_option;		/* option character returned by getopt_long(3) */
	int fd;						/* file descriptor */
	/* program name */

	program_name = argv[0]+2;

	/* file name */
	filename = (char *) ec_malloc(20);

	/* short options */
	const char *const short_options = ":f:s:";

	/* long options */
	const struct option long_options[] = {
		{ "seek-offset",	required_argument, NULL, 's' },
		{ NULL,						no_argument,			 NULL,  0  }
	};

	do {
		next_option = getopt_long(argc,argv,short_options,
																				long_options,NULL);
		switch(next_option) {
			case 's':	/* offset */
				offset = atoi(optarg);
				break;

			case 'f':	/* filename */
				/* File name specified. Learn it. */
				strcpy(filename,optarg);

			case -1:
				break;

			default:
				abort();
		}
	}
	while (next_option != -1);

	/*
	 * Open file.
	 */
	if ( (fd = open(FILE,
									O_CREAT|O_RDWR,
									S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) == -1) {
		perror(FILE);
		exit(EXIT_FAILURE);
	}
	
	/*** DEBUG ***/
	printf("[DEBUG] offset: %d\n",offset);
	printf("[DEBUG] fd:   %d\n",fd);

	/* read user uid */
	if ( read(fd,&uid,4) != 4 )			/* EOF */
		puts("EOF");

	/* Get the current offset after read(2). */


	/*** DEBUG ***/
	printf("[DEBUG] uid:   %d\n",uid);
	
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

/* Print error message and exit. */
void fatal(char *msg) {
	fprintf(stderr,"%s: ",program_name);
	perror(msg);
	exit(EXIT_FAILURE);
}

/* Handles malloc error-case. */
void *ec_malloc(unsigned int s) {
	void *p;
	if( (p = malloc(s) ) == NULL ) {
		fatal("Cann't allocate memory");
		exit(EXIT_FAILURE);
	}
	return p;
}
