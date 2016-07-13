/*
 * offset				
 *							--debug=char
 *     										h [104]  e [101]  l [108]
 *
 *							--debug=count
 *     										h [0]  e [1]  l [2]
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "debug.h"
#include "err.h"

char *program_name;

void usage(FILE* stream,int exit_code) {
	fprintf(stream,"Usage: %s [OPTIONS]...\n",program_name);
	fprintf(stream,"Provides visualisation for the read call. First file is opened and\n"
								 "cursor is set to the offset bytes. After if bytes count was specified with\n"
								 "-r option, read() is called and count bytes are requested, on return pf() is\n"
								 "called and print file contents starting at start(0) by default, proceeds to the\n"
								 "start of read() call. All characters are printed in dim. Characters affected by\n"
								 "read() call are printed in normal color. When the last byte affected by read() call\n"
								 "printed, proceeds printin in dim again till character described by condition is found\n"
								 "or offset becomes equal to end.\n"
								 "Otherwise if -r option was not specified, read() call is omitted and pf() is called, starts\n"
								 "prininting characters at offset specified by start in dim, till cursos position, after switching to\n"
								 "normal color and process printin till characters specified by cond is met or cursor offset becomes\n"
								 "equal to end.\n\n"
								 
								 "Mandatory arguments to long options, are mandatory for short options too.\n"
								 "  -o, --open=filename                       specify file name\n"
								 "  -f, --offset=count                        seek file count bytes\n"
								 "  -s, --start=offset                        specify start offset from which pf\n"
								 "                                            should start printing\n"
								 "  -e, --end=offset                          specify end offset on which pf() should\n"
								 "                                            stop printing\n"
								 "  -c, --cond=char                           specify character on which pf() should\n"
								 "                                            stop printing\n"
								 "  -n, --count=offset                        specify offset on which pf() should\n"
								 "                                            stop printing\n");
	exit(exit_code);
}


int main(int argc,char **argv) {

	int fd;								/* file descriptor */
	int next_option;			/* next option */
	int offset = 0;				/* seek offset */
	int start  = -1;				/* pf() start read */
	int end    = -1;				/* pf() end read */
	int count  = 0;				/* count bytes to read */
	char cond  = 0;				/* pf() matching stop condition */

	char *filename;				/* file name */
	char *buf;						/* read buffer */

	filename = (char *) ec_malloc(20);
	buf = (char *) ec_malloc(count);

	program_name = argv[0]+2;

	/* short options */
	const char *so="hf:o:s:e:c:n:";

	/* long options */
	const struct option lo[]={
		{ "help",				no_argument,			 NULL, 'h'},
		{ "open",				required_argument, NULL, 'o'},
		{ "offset",			required_argument, NULL, 'f'},
		{ "start",      required_argument, NULL, 's'},
		{ "end",        required_argument, NULL, 'e'},
		{ "cond",       required_argument, NULL, 'c'},
		{ "count",      required_argument, NULL, 'n'},
		{ NULL,											 		0, NULL,  0 }
	};

	do {
		next_option = getopt_long(argc,argv,so,lo,NULL);

		switch(next_option) {

			case 'h':		/* help */
				usage(stdout,EXIT_SUCCESS);

			case 'o':		/* open */
				if ( access(optarg,0) )	
					fatal(optarg);
				else
					strncpy(filename,optarg,strlen(optarg));
				break;

			case 'f':		/* offset */
				offset = atoi(optarg);
				break;

			case 'n':		/* read */
				count = atoi(optarg);
				break;
				
			case 's':		/* start */
				start = atoi(optarg);
				break;

			case 'e':		/* end */
				end = atoi(optarg);
				break;

			case 'c':		/* condition */
				cond = atoi(optarg);
				break;

			case -1:
				break;

			default:
				fprintf(stderr,"Try `%s --help' for more information.\n",program_name);
				exit(EXIT_FAILURE);
		}
	}
	while(next_option != -1);

	if ( *filename == 0 ) {		/* no file specified */
		fprintf(stderr,"%s: missing file name\n",program_name);
		exit(EXIT_FAILURE);
	}

	/* open file */
	if ( ( fd = open(filename,O_RDONLY) ) == -1 )
		fatal("Cann't open file");


	/* seek file */
	lseek(fd,offset,SEEK_CUR);

	if ( count > 0 )
		if ( ( count = read(fd,buf,count)) == -1 )
			fatal(filename);

	if ( start == -1 )		/* print from the beginning of the line */
		pf(fd,ls(fd,0),end,count,cond);
	else
		pf(fd,start,end,count,cond);
		
	printf("beginnig of the line:   %d\n",ls(fd,0));

	/* close file */
	if ( close(fd) == -1 )
		fatal(filename);

	return 0;
}
