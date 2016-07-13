#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include "headers.h"
#include "perm.h"

# define FILENAME			"/var/notes"
char *program_name;

void mask_check(const char *);

void usage(FILE *stream, int exit_code) {
	fprintf(stream, "Usage: %s [OPTIONS]...DATA\n", program_name);
	fprintf(stream, "Write data to '%s' till other file name specified with -o.\n\n",FILENAME);
	fprintf(stream, "Mandatory arguments to long options are mandatory for short options too.\n"
									"  -o, --output filename    specify filename to write data to\n" 
									"  -t, --truncate           truncate file to length of 0\n" 
									"  -c, --creat[=perm]       allow file creation\n");

	exit(exit_code);
}


/* 
 * MAIN
 */
int main(int argc,char *argv[]) {

	int userid,fd;			/* file descriptor */
	int next_option,flags,mode,truncate;
	char *buffer;

	/*** FLAGS ***/
	truncate = 0;

	/* Check for proper arguments count. */
	if (argc < 2)
		usage(stderr,EXIT_FAILURE);

	/* Initialize program name. */
	program_name = argv[0] + 2;

	buffer = (char *) ec_malloc(100);
	
	flags = O_WRONLY;								/* File access flags. */
	mode = 0;												/* File creation mode. */

	const char *const short_options=":tm:";

	/* long options */
	const struct option long_options[]={
		{ "truncate", no_argument, NULL, 't'},
	  { "mode",			required_argument, NULL, 'm'},
		{ NULL,				0, NULL,  0  }
	};

	do {
		next_option = getopt_long(argc,argv,short_options,
															long_options, NULL);

		switch(next_option) 
		{
				case 't':	
					/* Add O_TRUNC to file creation flags. */
					truncate++;
					flags |= O_TRUNC;
					break;


				case 'm':
					/* File creation mode specified. */
					mask_check(optarg);
					mode = perm(mode,optarg);
					break;
					
				case -1:
					break;

				default:
					abort();
		}
	}
	while (next_option != -1);

	/* Default file creation mode (S_IRUSR|S_IWUSR). */
	if (mode == 0)
		mode = 384;

	/* Check for file existence. */
	if ( access(FILENAME,F_OK) == -1 )
		if (creat(FILENAME,mode) == -1 )
			fatal("cannot create /var/notes");

	/* Append all data starting at optind. */
	for(;optind < argc; optind++) {
		strncat(buffer,argv[optind],strlen(argv[optind]));
		strncat(buffer," ",1);
	}


	/* If --truncate option was not specified open file in append flags. */
	if (truncate == 0)
		flags |= O_APPEND;

	/* Opening the file. */
	if ( (fd = open(FILENAME,flags)) == -1)
		fatal("cannot open /var/notes");

	/* Get the real user ID. */
	userid = getuid();

	/*** DEBUG ***/
	printf("[DEBUG] userid: %d\n", userid);


	/* Writing data. */
	if ( write(fd, &userid, 4) == -1 )			/* Write user ID before note data. */
		fatal(FILENAME);
	if ( write(fd,"\n",1)	== -1)											/* Terminate line. */
		fatal(FILENAME);

	if ( write(fd, buffer, strlen(buffer)) == -1)	/* Write note. */
		fatal(FILENAME);

	if ( write(fd,"\n",1)	== -1)		/* Terminate line. */
		fatal(FILENAME);
		

	/* Closing file. */
	if (close(fd) == -1)
		fatal(FILENAME);

	puts("Note has been saved.");

	free (buffer);

	return 0;
}

/* Check the proper length of permission mask. */
void mask_check(const char *mode) {
	if (strlen(mode) > 3) {
		fprintf(stderr,"%s: permissions mask to long.\n",program_name);
		fprintf(stderr,"Try `%s --help' for more information.\n",program_name);
		exit(EXIT_FAILURE);
	}
}							

