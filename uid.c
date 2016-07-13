#include <stdio.h>

int 
main() {
	int uid;
	int *ptr;
	
	uid = 10;
	ptr = &uid;

	printf("uid: %d\n",*ptr);
	return 0;
}
