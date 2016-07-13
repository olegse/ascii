/*
 * creat.c			- 		calculates access modes
 *
 */
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int nm(char *,int);			/* calculate flag value */
int perm(char *,int);		/* calculate mode specified by permissions mask */

void verbose(char *label,int flag) {
	//printf("permission bit: %d\t%s\n",flag,label);
	printf("%s:\t%d\n",label,flag);
}

int main(int argc,char **argv) {

	int nxt_opt;			/* next option */
	int mode  = 0;		/* mode value calculated */

	const char *short_opts=":f:m:";

	const struct option long_opts[]={
		{ "mode",     required_argument, NULL, 'm'},
		{ "flag",			required_argument, NULL, 'f'},
		{ "verbose",	no_argument,			 NULL, 'v'},
		{ NULL,											 		0, NULL,  0 }
	};

	do {
		nxt_opt = getopt_long(argc,argv,short_opts,long_opts,NULL);

		switch(nxt_opt) {
			case 'f':		
				mode = nm(optarg,mode);
				break;

			case 'm':
				mode = perm(optarg,mode);
				break;

			case -1:
				break;

			default:
				abort();
		}
	}
	while(nxt_opt != -1);

	printf("mode: %d\n",mode);
	return 0;
}

int nm(char *label,int mode) 
{
	int flag;

	if (strcmp(label,"O_APPEND") == 0)
		flag=O_APPEND;
	else if (strcmp(label,"O_ASYNC") == 0)
		flag=O_ASYNC;
	else if (strcmp(label,"O_CLOEXEC") == 0)
		flag=O_CLOEXEC;
	else if (strcmp(label,"O_CREAT") == 0)
		flag=O_CREAT;
	//else if (strcmp(label,"O_DIRECT") == 0)
	else if (strcmp(label,"O_DIRECTORY") == 0)
		flag=O_DIRECTORY;
	else if (strcmp(label,"O_EXCL") == 0)
		flag=O_EXCL;
	//else if (strcmp(label,"O_LARGEFILE") == 0)
	//else if (strcmp(label,"O_NOATIME") == 0)
	else if (strcmp(label,"O_NOCTTY") == 0)
		flag=O_NOCTTY;
	else if (strcmp(label,"O_NOFOLLOW") == 0)
		flag=O_NOFOLLOW;
	else if (strcmp(label,"O_NONBLOCK") == 0)
		flag=O_NONBLOCK;
	else if (strcmp(label,"O_NDELAY") == 0)
		flag=O_NDELAY;
	else if (strcmp(label,"O_SYNC") == 0)
		flag=O_SYNC;
	else if (strcmp(label,"O_WRONLY") == 0)
		flag=O_WRONLY;
	else if (strcmp(label,"O_RDONLY") == 0)
		flag=O_RDONLY;
	else if (strcmp(label,"O_RDWR") == 0)
		flag=O_RDWR;
	else if (strcmp(label,"O_TRUNC") == 0)
		flag=O_TRUNC;
	else
	{
		/* Invalid label name. */
		fprintf(stderr,"Unrecognized label: %s\n",label);
		exit(EXIT_FAILURE);
	}

	/* print flag value */
	verbose(label,flag);
	return mode |= flag;
}


int perm(char *mask,int mode) {
	int flag, n;
	char *cp,	*label;

	n = 0;

	for(cp = mask; *cp != '\0'; ++cp)
	{

		/* DEBUG */
		printf("Processing: %c\n",*cp);

		switch(n) 
		{
			case 0:

				/*
				 * user permissions 
				 */

				switch(*cp) 
				{
					case '7': flag=S_IRWXU; 				label="S_IRWXU"; 				 break;
					case '6': flag=S_IRUSR|S_IWUSR; label="S_IRUSR|S_IWUSR"; break;
					case '5': flag=S_IRUSR|S_IXUSR; label="S_IRUSR|S_IXUSR"; break;
					case '4': flag=S_IRUSR; 				label="S_IRUSR"; 				 break;
					case '3': flag=S_IWUSR|S_IXUSR; label="S_IWUSR|S_IXUSR"; break;
					case '2': flag=S_IWUSR; 				label="S_IWUSR"; 				 break;
					case '1': flag=S_IXUSR; 				label="S_IXUSR"; 				 break;
					case '0':	continue;
				}
				break;
			
			case 1:

				/*
				 * group permissions
				 */

				switch(*cp)
				{
					case '7': flag=S_IRWXG; 				label="S_IRWXG"; 				 break;
					case '6': flag=S_IRGRP|S_IWGRP; label="S_IRGRP|S_IWGRP"; break;
					case '5': flag=S_IRGRP|S_IXGRP; label="S_IRGRP|S_IXGRP"; break;
					case '4': flag=S_IRGRP; 				label="S_IRGRP"; 				 break;
					case '3': flag=S_IWGRP|S_IXGRP; label="S_IWGRP|S_IXGRP"; break;
					case '2': flag=S_IWGRP; 				label="S_IWGRP"; 				 break;
					case '1': flag=S_IXGRP; 				label="S_IXGRP"; 				 break;
					case '0':	continue;
				}
				break;

			case 2:

				/*
				 * other permissions
				 */

				switch(*cp)
				{
					case '7': flag=S_IRWXO; 				label="S_IRWXO"; 				 break;
					case '6': flag=S_IROTH|S_IWOTH; label="S_IROTH|S_IWOTH"; break;
					case '5': flag=S_IROTH|S_IXOTH; label="S_IROTH|S_IXOTH"; break;
					case '4': flag=S_IROTH; 				label="S_IROTH"; 				 break;
					case '3': flag=S_IWOTH|S_IXOTH; label="S_IWOTH|S_IXOTH"; break;
					case '2': flag=S_IWOTH; 				label="S_IWOTH"; 				 break;
					case '1': flag=S_IXOTH; 				label="S_IXOTH"; 				 break;
					case '0':	continue;
				}
				break;

		}

		verbose(label,flag);
		mode |= flag;
		n++;
	}

	return mode;
}
