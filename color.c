#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char **argv) {
	int i,fd;

	if ( (fd = open("color_files",O_WRONLY|O_CREAT,S_IRWXU|S_IRUSR|S_IWUSR,S_IROTH|S_IWOTH)) != -1 ) {
		for (i=0; i < 256; ++i) {
		if ( write(fd,"\\t\\033[%dmTTTTTTT\\033\[0m\n",100) == -1 )
			perror("/var/notes/");
			exit(1);
		}
	}
	return 0;
}
