/* 
 * ls():		returns line start offset
 */
int ln(int fd,int offset) {
	int c;			/* read() return code */
	int cur;		/* stores cursor offset before an execution */
	int beg;		/* stores beginning of the line offset */
	char byte;	/* read() place holder */

	cur = lseek(fd,0,SEEK_CUR);

	if ( offset != 0 )
		/* start calculating from the offset */
		lseek(fd,offset,SEEK_SET);
		
	/*** DEBUG ***/
	printf("[DEBUG] starting at :   %d\n", offset);

	/* Seek one byte further. */
	lseek(fd,1,SEEK_CUR);
		
	do {

		/* Seek two bytes backwards. */
		lseek(fd,-2,SEEK_CUR);

		if ( ( c = read(fd,&byte,1) == -1 ) ) /* error */
			fatal("Cann't read data");

	} while ( byte != '\n' );

	beg =  lseek(fd,0,SEEK_CUR);	/* read line start offset */
	lseek(fd,cur,SEEK_SET);				/* reset offset to initial state */

	return beg;
}
