/*
 * char.c				characters parser
 */
#include <stdio.h>

int chr(char *s,int index);			/* prints characters in dim from the beginning
																	 till index and proceeds printing in regular color */

int main() {
	chr("hello jopa\n",3);
	return 0;
}

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
	
	return i;
}
