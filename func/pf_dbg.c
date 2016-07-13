/* 
 * pf_dbg():		print offset with different highlighting in between beginning of the line
 *							start of reading and offset after reading 
 *			
 *							Prints debugging information.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int pf(int fd,int start,int count,int condition) {

	int step,offset,rs;
	char byte = -1;

	/* Remeber current offset after last read() call. */
	offset = lseek(fd,0,SEEK_CUR);

	/* Calculate offset before last read() call. */
	rs = offset - count;

	/* Start reading from offset specified by start (0 by default). */
	step = lseek(fd,start,SEEK_SET);

	printf("[DEBUG] fd:      %d\n",fd);
	printf("[DEBUG] start:   %d\n",start);
	printf("[DEBUG] offset:  %d\n",offset);
	printf("[DEBUG] rs:      %d\n",rs);
	printf("[DEBUG] step:    %d\n",step);
	printf("[DEBUG] count:   %d\n",count);


	while ( (count = read(fd,&byte,1)) > 0 ) {		/* loop till end of line reached */
		/*
		 * Error while read()
		 */
		if ( count == -1 ) {
			perror("Cann't read data");
			exit(EXIT_FAILURE);
		}

		/*
		 * Matched character describing end of read condition.
		 */
		if ( byte == condition ) 
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
