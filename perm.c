/* 
 * perm():			return calculated permissions mask for use as mode argument in open() and creat() 
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
int perm(unsigned int mode, char *mask) {
	int n;		/* permissions group index */
	char *cp;				/* char pointer */

	n = 0;

	for(cp = mask; *cp != '\0'; ++cp)
	{

		switch(n) 
		{
			case 0:

				/*
				 * user permission 
				 */

				switch(*cp) 
				{

					case '7': mode |= S_IRWXU; 				 break;
					case '6': mode |= S_IRUSR|S_IWUSR; break;
					case '5': mode |= S_IRUSR|S_IXUSR; break;
					case '4': mode |= S_IRUSR; 				 break;
					case '3': mode |= S_IWUSR|S_IXUSR; break;
					case '2': mode |= S_IWUSR; 				 break;
					case '1': mode |= S_IXUSR; 				 break;
					case '0': continue;

					default:	abort();
				}
				break;
			
			case 1:

				/*
				 * group permission
				 */

				switch(*cp)
				{
					case '7': mode |= S_IRWXG; 				 break;
					case '6': mode |= S_IRGRP|S_IWGRP; break;
					case '5': mode |= S_IRGRP|S_IXGRP; break;
					case '4': mode |= S_IRGRP; 				 break;
					case '3': mode |= S_IWGRP|S_IXGRP; break;
					case '2': mode |= S_IWGRP; 				 break;
					case '1': mode |= S_IXGRP; 				 break;
					case '0': continue;

					default:	abort();
				}
				break;

			case 2:

				/*
				 * other permission
				 */

				switch(*cp)
				{
					case '7': mode |= S_IRWXO; 				 break;
					case '6': mode |= S_IROTH|S_IWOTH; break;
					case '5': mode |= S_IROTH|S_IXOTH; break;
					case '4': mode |= S_IROTH;  			 break;
					case '3': mode |= S_IWOTH|S_IXOTH; break;
					case '2': mode |= S_IWOTH; 				 break;
					case '1': mode |= S_IXOTH; 				 break;
					case '0': continue;

					default:	abort();
				}
				break;

		}

		/* process next permissions group */
		n++;
	}

	return mode;
}
