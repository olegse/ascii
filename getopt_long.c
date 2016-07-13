/*
 * Check if file exist.
 */
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc,char **argv) {
		
	int next_option;

	const char *const short_options = ":f:";

	const struct option long_options[] = {
		{ "file",					required_argument, NULL, 'f' },
		{ NULL,						no_argument,			 NULL,  0  }
	};

	do {
		next_option = getopt_long(argc,argv,short_options,
																				long_options,NULL);

		if (next_option == -1)
			printf("next_option:   %d\n", next_option);
		else
			printf("next_option:   %c\n", next_option);
		printf("optarg:        %s\n", optarg);
		printf("optind:        %d\n", optind);
		printf("optopt:        %c\n", optopt);

		printf("---------------------------------\n");
	}
	while(next_option != -1);

	return 0;
}
