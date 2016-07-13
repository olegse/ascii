/*
 * Check if file exist.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

const char *program_name;

/* Error-checked malloc wrapper function. */
void *ec_malloc(unsigned int);

/* Print usage to stream and exit with exit_code. Does not return. */
void *usage(FILE* stream,int exit_code) {
	fprintf(stream,
					"Usage: %s FILE...\n"
					"Report file existense.\n"
					"Mandatory arguments to long options are mandatory for short options too.\n" 
					"  -f, --file filename        specify file name to test\n"
					"  -h, --help                 print usage and exit\n", program_name);
	exit(exit_code);
}

int main(int argc,char **argv) {
		
	int next_option;
	char *file;

	program_name=argv[0]+2;						/* program name */
	file = (char *) ec_malloc(20);		/* file name */

	const char *const short_options = ":f:h";

	const struct option long_options[] = {
		{ "file",			required_argument, NULL, 'f' },
		{ "help",					no_argument,			 NULL, 'h' },
		{ NULL,						no_argument,			 NULL,  0  }
	};

	do {
		next_option = getopt_long(argc,argv,short_options,
																				long_options,NULL);
		switch(next_option) {
			case 'f':
				/* Filename specified. Store it in file. */
				strcpy(file,optarg);
				break;

			case 'h':
				/* Print usage message and exit. */
				usage(stdout,EXIT_SUCCESS);
				break;

			case '?':
				/* Invalid argument encountered. */
				fprintf(stderr,
								"%s: Invalid option -- '%c'\n", program_name, optopt);
				exit(EXIT_FAILURE);
			
			case ':':
				/* Missing option argument. */
				fprintf(stderr,
								"%s: Option '%s' requires an argument\n"
								"Try `%s --help' for more information.\n",program_name,argv[optind-1],
																													program_name);
				exit(EXIT_FAILURE);

			case -1:
				break;

			default:
				abort();
		}
	}
	while (next_option != -1);

	/* File should be specified. */
	if (*file == 0) {
		fprintf(stderr,"%s: Please init the file.\n"
									 "Try `%s --help' for more information.\n", program_name,program_name);
		exit(EXIT_FAILURE);
	}

	/*** DEBUG ***/
	printf("[DEBUG] file:  %s [%p]\n",file,file);

	printf("access to the file: %d\n",access(file,F_OK));
	return 0;
}

/* ec_malloc() */
void *ec_malloc(unsigned int size) {
	void *ptr;
	if( (ptr = malloc(size)) == NULL) {
		fprintf(stderr,"%s: ",program_name);
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
