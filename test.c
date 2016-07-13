#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char **argv) {
	printf("real user id:   %d\n",getuid());
	printf("effective user id:   %d\n",geteuid());

	return 0;
}
