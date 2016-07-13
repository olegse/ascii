/*
 * notesearch.c  			find note 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "debug.h"
#include "err.h"

#define 	FILENAME	"/var/notes"

int print_notes(int, int, char *);			/* Note printing function. */
int find_user_note(int, int);						/* Seek in file for a note for user. */
int search_note(char*,char*);						/* Search for keyword function. */

char *program_name;			/* program name */

/* Print usage to stream and exit with exit code. Does not return. */
void usage(FILE* stream,int exit_code) {
	fprintf(stream,"Usage: %s [OPTIONS]...\n",program_name);
	fprintf(stream,"Looking for a note in /var/notes/ prefixed with the user id running a program.\n"
								 "If search string does not found on command line note will be printed \n\n"
								 "Mandatory arguments to long options are mandatory for short options too.\n" 
								 "  -s, --search-string=string\n"
								 															"specify search string\n\n"
								 "  -h, --help                 print this help\n");
	exit(exit_code);
}
								 
/*
 * MAIN
 */
int main(int argc,char *argv[]) {
	int next_option;											/* next option */
	int userid, printing=1, fd;						/* File descriptor. */
	char searchstring[100];								/* keyword */

	/* init program name */
	program_name=argv[0]+2;

	/* short options */
	const char *short_options = ":hs:";

	/* long options */
	const struct option long_options[] = {
		{ "search-string",			required_argument, NULL, 's' },
		{ "help",								no_argument,			 NULL, 'h' },
		{ NULL,									0,								 NULL,  0  }
	};

	/* Options parsing */
	do {
		next_option = getopt_long(argc,argv,short_options,long_options,NULL);

		switch(next_option) {
			case 's':		/* search-string */
				strncpy(searchstring,optarg,strlen(optarg));
				break;

			case 'h':		/* help */
				usage(stdout,EXIT_SUCCESS);

			case '?':
				usage(stderr,EXIT_FAILURE);

			case -1:		/* done with option */
				break;

			default:		/* something else: unexpected */
				abort();
		}
	}
	while(next_option != -1);

	/* Check for file existence. */
	if ( access(FILENAME,F_OK) == -1 ) {
		fprintf(stderr,"%s does not exist. First write some notes with notetaker.\n", FILENAME);
		exit(EXIT_FAILURE);
	}

	/* If any argument found, threat it as a search string, oterwise
	   search string is empty. */
	if (argc > 1)
		strcpy(searchstring, argv[1]);
	else
		searchstring[0] = 0;		/* NULL */

	/*** DEBUG ***/
	printf("[DEBUG] searchstring:   %s\t[%p]\n",searchstring,searchstring);


	/* UID */
	userid = getuid();

	/*** DEBUG ***/
	printf("[DEBUG] userid:   %d\n",userid);

	if ( (fd = open(FILENAME, O_RDONLY)) == -1)				/* Open the file for read only access. */
		fatal(FILENAME);

	/* printing is set to 1, that means condition is true */
	while(printing)
		printing = print_notes(fd,userid,searchstring);
	printf("---------[ end of note data ]----------\n");

	if (close(fd) == -1)
		fatal(FILENAME);

	return 0;
}

/* A function to print the notes for a given uid that match an optional search string.
   Returns 0 at the end of file, 1 if there are still more notes. */
int print_notes(int fd, int uid, char *searchstring) {
	int note_length;
	char note_buffer[100];

	/* Find a note length. */
	note_length = find_user_note(fd,uid);
	printf("[DEBUG] note length:   %d\n", note_length);

	if(note_length == -1)		/* If end of file was reached, return 0. */
		return 0;
	else if ( ! note_length ) {
		printf("No notes found for user id:   %d\n",uid);
		return 0;
	}
	
	read(fd,note_buffer,note_length);			/* Read note data. */

	printf("[DEBUG] Reading note: \n");
	printf("[DEBUG] offset:   %d\n",(int)lseek(fd,0,SEEK_CUR));
	printf("[DEBUG] line start:   %d\n",ls(fd,45));
	pf(fd,ls(fd,lseek(fd,0,SEEK_CUR)-1),note_length,0); 
	note_buffer[note_length] = 0;					/* Terminate the string. */

	if( search_note(note_buffer,searchstring))	/* If searching found, print the note. */
		printf(note_buffer);
	return 1;
}

/* A function to find the next note for a given userID. Returns -1 if the end
   of file is reached, otherwise returns the length of the found note. */
int find_user_note(int fd, int user_id) {
	int note_uid=-1;
	unsigned char byte;
	int length;
	int beg;				/* Tracks beginning of the line. */

	/*** DEBUG ***/
	printf("[DEBUG] Searching for note for uid: %d\n",user_id);

	while(note_uid != user_id) {		/* Loop until a note for user_uid is found. */
		/* Read uid data. If 4 bytes aren't read, return end of file code. */
		if ( read(fd, &note_uid, 4) != 4 )	
			return -1;

		/*** DEBUG ***/
		printf("[DEBUG] uid found: %d\n",note_uid);

		/* Read new line separator. If byte wasn't read return end of 
			 file code. */
		if (read(fd,&byte,1) != 1)
			return -1;

		beg = ls(fd,0);
		/*** DEBUG ***/
		printf("[DEBUG] beg:   %d\n", beg);

		/* Figure out how many bytes to the end of the line. */
		byte = length = 0;
		while(byte != '\n') {

			/* Read a single byte. If a byte isn't read return end of file code. */
			if(read(fd,&byte,1) != 1)
				return -1;			/* EOF */

			if ( byte == 10 )
				printf("read : \\n\n");
			else
				printf("read: %c\n",byte);

			pf(fd,beg,0,10);
			length++;	
		}

		/* Found note id matching current real id. */
		if ( note_uid == user_id ) {

			/*** DEBUG ***/
			printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);

			/* Rewind file reading by length bytes. */
			lseek(fd,length * -1, SEEK_CUR);	

			/*** DEBUG ***/
			pf(fd,beg,0,10);

			/* Return length to be used by next read call. */
			return length;
		}
	}
	return 0;
}

/* A function to search a note for a given keyword. Returns 1 if a match found, 0 if there is no
	 match. */
int search_note(char *note,char *keyword) {
	int i, keyword_length, match=0;

	keyword_length = strlen(keyword);
	if (keyword_length == 0)		/* If there is no search string, always match. */
		return 1;
	
	for (i=0; i < strlen(note); i++) 	/* Iterate over bytes in note. */
		if (note[i] == keyword[match])	/* Byte matches keyword, proceeding to the next byte. */
			match++;
		else {
			if (note[i] == keyword[0])		/* If byte matches first keyword byte, start the match cnt at 1. */
				match = 1;
			else
				match = 0;									/* Otherwise it is zero. */
		}

	if (match == keyword_length)		/* Full match. */
		return 1;
	
	return 0;		/* Return not matched. */
}
