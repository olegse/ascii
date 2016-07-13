/*
 * matching		-		attempt to provide visualisation of character matching alghoritms
 */
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "debug.h"

char *program_name;

int main(int argc,char **argv) {
	int i,m;
	int match = 0;
	char *buf;
	char *keyword;

	program_name = argv[0] + 2;

	buf = (char *) ec_malloc(20);
	keyword = (char *) ec_malloc(20);

	strcpy(buf,"hello world world");
	strcpy(keyword,"wo");

	for (i=0; buf[i] != 0; i++)
	{
		printf("\033[2m");		/* print dim */

		for(m=0; m < i; m++ )
		{
			
			if( buf[m] == keyword[match] ) {
				match++;
				printf("\033[31m");		/* print red */
			}
			else {
				match=0;
				printf("\033[0m");		/* reset color */
			}

			
			if ( buf[m] == '\n' )
				printf("\\n \n");
				
			else if ( buf[m] == ' ' )
				printf("[ ] ");
			else
				printf("%c ",buf[m]);

		}

		printf("\033[0m");		/* reset color */

		for(; buf[m] != 0; m++ )
			if ( buf[m] == '\n' )
				printf("\\n\n");
			else if ( buf[m] == ' ' )
				printf("[ ] ");
			else
				printf("%c ",buf[m]);
		printf("\n");
		
	}
	
	return 0;
}
