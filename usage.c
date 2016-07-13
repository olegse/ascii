void usage(FILE *stream, int exit_code) {
	fprintf(stream, "Usage: %s [OPTIONS]...DATA\n", program_name);
	fprintf(stream, "Write data to '%s' till other file name specified with -o.\n\n",FILENAME);
	fprintf(stream, "Mandatory arguments to long options are mandatory for short options too.\n"
									"  -o, --output filename    specify filename to write data to\n" 
									"  -t, --truncate           truncate file to length of 0\n" 
									"  -c, --creat[=perm]       allow file creation\n");

	exit(exit_code);
}

