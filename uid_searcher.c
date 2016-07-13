	if ( (c = read(fd,&uid,4)) != 4 )			/* EOF */
	printf("[DEBUG] uid:   %d\n",uid);
