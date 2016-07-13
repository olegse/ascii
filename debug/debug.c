#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "err.h"

/* 
 * pf():		print offset with different highlighting in between beginning of the line
 *					start of reading and offset after reading 
 */
int pf(int fd,int start,int end,int count,int cond) {

	int step,offset,rs;
	char byte = -1;

	/* Remeber current offset after last read() call. */
	offset = lseek(fd,0,SEEK_CUR);

	/* Calculate offset before last read() call. */
	rs = offset - count;

	/* Start reading from offset specified by start (0 by default). */
	step = lseek(fd,start,SEEK_SET);

	printf("end:   %d\n",end);
	while ( (count = read(fd,&byte,1)) > 0 ) {		/* loop till end of line reached */
		/*
		 * Error while read()
		 */
		if ( count == -1 ) {
			perror("Cann't read data");
			exit(EXIT_FAILURE);
		}

		/*
		 * Matched character describing end of read condition or offset specified by read is reached.
		 */
		if ( byte == cond || step == end )
			break;

		/* 
		 * Print data.
		 */

		/* Read from offset after previous read() call till condition is met. */
		if (step >= offset) {

			printf("\033[0m");		/* reset any color */

			if ( rs == offset ) 			/* no previous read() call (count == 0)*/
				if ( byte == '\n' )
					printf("\\n\n");
				else if ( byte == ' ' )
					printf("[ ] ");
				else
					printf("%c ",byte);
			else {

				printf("\033[2m");	/* dim */

				if (byte == '\n') 
					printf("\\n\n");
				else if ( byte == ' ' )
					printf("[ ] ");
				else
					printf("%c ",byte);
			}
		}

		/* Read from offset before the previous read() call. Color: normal. */
		else if ( step >= rs ) {
			printf("\033[0m");	/* reset previous color */
			if ( byte == '\n' )	/* new line */
				printf( "\\n\n" );
			else if ( byte == ' ' )
				printf("[ ] ");
			else
				printf("%c ",byte);
		}
		
		/* Reading from the offset specified by start till offset
			 where previous read() call occured. Color: dim (\033[2m) */
		else {

			printf("\033[0m\033[2m");			/* always dim from the beginning */
			if (byte == '\n')
				printf("\\n\n");
			else if ( byte == ' ' )
				printf("[ ] ");
			else
				printf("%c ",byte);

		}
		step++;
	}
	printf("\033[0m");	/* reset color */
	printf("\n");			/* print new line */

	/* Reset offset back. Was calculated at the beginning. */
	return lseek(fd,offset,SEEK_SET);
}


/* 
 * ls():		returns line start offset
 */
int ls(int fd,int offset) {
	int c;			/* read() return code */
	int cur;		/* stores cursor offset before an execution */
	int beg;		/* stores beginning of the line offset */
	char byte;	/* read() place holder */

	/* Remember cursor position. */
	cur = lseek(fd,0,SEEK_CUR);
	beg = 0;

	if ( offset != 0 )
		/* start calculating from the offset */
		lseek(fd,offset,SEEK_SET);
		
	/* Seek one byte further. */
	lseek(fd,1,SEEK_CUR);
		
	do {

		/* Seek two bytes backwards. */
		if ( lseek(fd,-2,SEEK_CUR) == -1 ) {
			
			/* Beginning of the line was reached. */
			beg = 1;
			break;
		}

		if ( ( c = read(fd,&byte,1) == -1 ) ) /* error */
			fatal("Cann't read data");

	} while ( byte != '\n' );

	beg = lseek(fd,0,SEEK_CUR) - beg;	/* read line start offset */
	lseek(fd,cur,SEEK_SET);						/* reset offset to initial state */

	return beg;
}


/*
 * char.c				prints dim characters till ind is reached; return string length
 */

int chr(char *s,int ind) {

	int i;

	printf("\033[2m");		/* print dim */

	for(i=0; i < ind; i++ )
		if ( s[i] == '\n' )
			printf("\\n\n");
		else if ( s[i] == ' ' )
			printf("[ ] ");
		else
			printf("%c ",s[i]);


	printf("\033[0m");		/* reset color */

	for(; s[i] != 0; i++ )
		if ( s[i] == '\n' )
			printf("\\n\n");
		else if ( s[i] == ' ' )
			printf("[ ] ");
		else
			printf("%c ",s[i]);

	printf("\n");
	
	return i;
}
