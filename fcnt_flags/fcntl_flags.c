#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void display_flags(char *,unsigned int,int);
void binary_print(unsigned int);

int main(int argc,char *argv[]) {
	int op;				/* option */
	int binary=0;		/* binary_print() trigger */

	/* short options */
	const char *const shrt_op="b";

	do {
		op=getopt(argc,argv,shrt_op);

		switch(op) {
			case 'b':
				binary++; break;

			case -1:
				break;

			default:
				abort();
		}
	}
	while(op != -1);

	display_flags("O_RDONLY\t\t", O_RDONLY, binary);
	display_flags("O_WRONLY\t\t", O_WRONLY, binary);
	display_flags("O_RDWR\t\t", O_RDWR, binary);
	printf("\n");
	display_flags("O_APPEND\t\t", O_APPEND, binary);
	display_flags("O_TRUNC\t\t", O_TRUNC, binary);
	display_flags("O_CREAT\t\t", O_CREAT, binary);
	printf("\n");
	display_flags("S_IRWXU\t\t", S_IRWXU, binary);
	//display_flags("O_WRONLY|O_APPEND|O_CREAT", O_WRONLY|O_APPEND|O_CREAT,binary);

	return 0;
}

void display_flags(char *label, unsigned int value, int binary) {
	printf("%s\t: %d\t", label, value);
	if (binary)
		binary_print(value);
	printf("\n");
}

void binary_print(unsigned int value) {
	unsigned int mask = 0xff000000;		/* start with a mask for the highest byte */
	unsigned int shift = 256*256*256;		/* start with a shift for the highest byte */
	unsigned int byte, byte_iterator, bit_iterator;

	for(byte_iterator=0; byte_iterator < 4; byte_iterator++) {
		byte = (value & mask ) / shift;		/* isolate each byte */
		printf(" ");
		for(bit_iterator=0; bit_iterator < 8; bit_iterator++) {		/* print the byte's bits */
			if(byte & 0x80)			/* if the highest bit in the byte isn't 0, */
				printf("1");			/* print 1 */
			else
				printf("0");			/* otherwise print 0 */
			byte *= 2;					/* move all the bits the left by 1 */
		}
		mask /= 256;					/* move the bits in mask right by 8 */
		shift /=256;					/* move the bits in shift right by 8 */
	}
}
