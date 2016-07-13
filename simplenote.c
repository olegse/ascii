/*
 * simplenote.c 			write data (found on command line to /tmp/notes
 *
 * by default append data
 */
#include <stdio.h>
#include <stdlib.h>			/* for exit(3) */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>			/* for write(2) */
#include <getopt.h>
#include "perm.h"

const char *program_name;		/* program name */
char *file;									/* file name */

void fatal(char *);												/* Print error message and exit. */
void *ec_malloc(unsigned int);						/* Error-checked malloc() wrapper. */

void usage(FILE *stream, int exit_code) {
	fprintf(stream, "Usage: %s [OPTIONS]...DATA\n", program_name);
	fprintf(stream, "Write data to '/tmp/notes/' till other file name specified with -o.\n"
									"\n"
									"Mandatory arguments to long options are mandatory for short options too.\n"
									"  -o, --output filename    specify filename to write data to\n"
									"  -t, --truncate           truncate file to length of 0\n"
									"  -c, --creat[=perm]      allow file creation\n");

	exit(exit_code);
}

/* print flag labels that will be used with open(2) and 
	 corresponding values */
void verbose(char *,unsigned int);

/* 
 * MAIN
 */
int main(int argc,char *argv[]) {

	int nxt_opt;			/* next option */

	int fd;						/* file descriptor */

	int mode;					/* permissions mode */
	int access_mode;	/* access mode */
	int creat = 0;		/* create file trigger */
	int truncate = 0;	/* truncate file trigger */
	
	char *buffer;			/* data buffer */

	mode = 0;

	/* Allocate memory for the data buffer. */
	buffer = ec_malloc(100);

	/* Initialize file name. */
	file = ec_malloc(20);
	strncpy(file,"/tmp/notes",20);

	/* advance argv[0] by two bytes and store a 
		 pointer in program name */
	program_name = argv[0] + 2;


	/* Print usage if no command line arguments specified. */
	if(argc == 1)
		usage(stdout,EXIT_SUCCESS);

	/* Access mode. */
	access_mode = O_WRONLY;


	/* short options */
	const char *const shrt_opt="o:c::t";

	/* long options */
	const struct option lng_opt[]={
		{ "output",		required_argument, NULL, 'o'},
		{ "creat",		optional_argument, NULL, 'c' },
		{ "truncate", no_argument, NULL, 't'},
		{ NULL,				0, NULL,  0  }
	};


	do {
		nxt_opt=getopt_long(argc,argv,shrt_opt,
																	lng_opt,NULL);

		switch(nxt_opt) {
			case 'c':
				//chk_msk(optarg);

				if (optarg != NULL) {
					/* Check if the permissions mask is not more three chars (bits). */
					if ( strlen(optarg) > 3 ) {
						fprintf(stderr, "Permissions mask is too long\n");
						exit(EXIT_FAILURE);
					}

					/* Calculate mode for permissions mask (optarg). */
					mode = perm(mode,optarg);
				}
				else
					/* Default creation mode. */
					mode = S_IRWXU|S_IRGRP|S_IROTH;
					
				creat++;
				access_mode |= O_CREAT;
				break;

			case 'o':
				strcpy(file,optarg);
				break;

			case 't':			/* truncate file to 0 length before writing */
				truncate++;
				access_mode |= O_TRUNC;
				break;

			case 'h':
				usage(stdout,0);

			case -1:
				break;

			default:
				abort();
		}
	}
	while (nxt_opt != -1);


	/* Append data till -t. */
	if (truncate == 0)
		access_mode |= O_APPEND;

	/*
	 * Opening a file.
	 */


	/* DEBUG */
	printf("[DEBUG] mode: %d\n",mode);

	if (creat == 1) {		/* create */
		printf("Creating file...\n");
		fd = open(file,access_mode,mode);
	}
	else
		fd = open(file,access_mode);
	
	/* Error check. */
	if (fd == -1)
		fatal("open()");

	/*
	 * DEBUG
	 */
	printf("[DEBUG] file: %s\t[%p]\n",file,file);
	printf("[DEBUG] fd: %d\n", fd);


	for(;optind < argc-1; optind++) {
		/* 
		 * Concatenate all (data-)elements, starting at optind into buffer.
		 * Add a space after each element excluding last. (optind < argc-1).
		 */
		
		strncat(buffer,argv[optind],strlen(argv[optind]));
		strncat(buffer," ",1);
	}

	/* Write last element and add a new line. */
	strncat(buffer,argv[optind],strlen(argv[optind]));
	strncat(buffer,"\n",1);
	

	/* 
	 * Write buffer.
	 */

	if(write(fd,buffer,strlen(buffer)) == -1)
		fatal("write()");

	close(fd);
	free(file);

	return 0;
}

/* Print msg followed by an error returned by last function or library code
 	 and exit with code. Does not return. */
void fatal(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/*
 * ec_malloc():
 */
void *ec_malloc(unsigned int s) {
	void *p;
	p = malloc(s);

	if (p == NULL) 
		fatal("malloc()");
	return p;
}
