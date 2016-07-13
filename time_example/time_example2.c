#include <stdio.h>
#include <time.h>

void dump_time_struct_bytes(struct tm *time_ptr,int size) {
	int i;
	unsigned char *raw_ptr;

	printf("bytes of struct located at 0x%08x\t",(unsigned int) time_ptr);
	raw_ptr = (unsigned char *) time_ptr;

	for(i=0; i < size; i++)
	{
		printf("%02x \033[2m[%d]\033[0m  ",raw_ptr[i],i);
		if(i%16 == 15)			/* print a new line every 16 bytes */
			printf("\n");
	}
	printf("\n");
}

void dump_time_struct_int(struct tm *time_ptr,int size) {
	int i;
	unsigned char *raw_ptr;
	int *int_ptr;			/* each 4 iterations retrieve inteeger */

	int_ptr = (int *) (raw_ptr = (unsigned char *) time_ptr);

	for(i=0; i < size; i++) {
		printf("%02x \033[2m[%2d]\033[0m  ",raw_ptr[i],i);
		if( i % 4 == 3 ) {
			printf("\t%d\n",*int_ptr);
			int_ptr++;
		}
	}
		
}
	
/* think about:			address:   0x02 0x00 0x00 0x00      int (highlighted) */
int main() {
	long int seconds_since_epoch;
	struct tm current_time, *time_ptr;

										/* time_t time(time_t *t); */
	seconds_since_epoch = time(0);		/* Pass time a null pointer as argument */
	printf("time() - seconds since epoch: %ld\n",seconds_since_epoch);

	time_ptr = &current_time;				/* Set time ptr to the address of the current_time struct */

	/* struct tm localtime_r(const time_t *timep,struct tm *result); */
	localtime_r(&seconds_since_epoch,time_ptr);

	dump_time_struct_bytes(time_ptr,sizeof(struct tm));
	dump_time_struct_int(time_ptr,sizeof(struct tm));

	return 0;
}
