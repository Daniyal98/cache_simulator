#include <stdio.h>
//#include <unistd.h> //for the getopt function
#include <stdlib.h> //for atoi
#include <iostream>
using namespace std;


int     opterr = 1,             /* if error message should be printed */
optind = 1,             /* index into parent argv vector */
optopt,                 /* character checked for validity */
optreset;               /* reset getopt */
char    *optarg;
#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

int getopt(int nargc, char * const nargv[], const char *ostr)
{
	static char *place = EMSG;              /* option letter processing */
	const char *oli;                        /* option letter list index */

	if (optreset || !*place) {              /* update scanning pointer */
		optreset = 0;
		if (optind >= nargc || *(place = nargv[optind]) != '-') {
			place = EMSG;
			return (-1);
		}
		if (place[1] && *++place == '-') {      /* found "--" */
			++optind;
			place = EMSG;
			return (-1);
		}
	}                                       /* option letter okay? */
	if ((optopt = (int)*place++) == (int)':' ||
		!(oli = strchr(ostr, optopt))) {
		/*
		* if the user didn't specify '-' as an option,
		* assume it means -1.
		*/
		if (optopt == (int)'-')
			return (-1);
		if (!*place)
			++optind;
		if (opterr && *ostr != ':')
			(void)printf("illegal option -- %c\n", optopt);
		return (BADCH);
	}
	if (*++oli != ':') {                    /* don't need argument */
		optarg = NULL;
		if (!*place)
			++optind;
	}
	else {                                  /* need an argument */
		if (*place)                     /* no white space */
			optarg = place;
		else if (nargc <= ++optind) {   /* no arg */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)printf("option requires an argument -- %c\n", optopt);
			return (BADCH);
		}
		else                            /* white space */
			optarg = nargv[optind];
		place = EMSG;
		++optind;
	}
	return (optopt);                        /* dump back option letter */
}
bool parseParams(int argc, char *argv[], int& cache_capacity,
	int& cache_blocksize, int& cache_associativity,bool& split,char&hwrite,char&mwrite)
{
	//needed for the parsing of command line options
	int c;
	bool c_flag, b_flag, a_flag;
	bool errflg = false;

	c_flag = b_flag = a_flag = errflg = false;

	//the following variables are used by getopt and
	//are defined elsewhere so we just make them
	//extern here
	extern char *optarg;
	extern int optind, optopt;

	//start the parsing of the command line options.
	//end is indicated by getopt returning -1
	//each option has a case statement
	//the corresponding flags are set if the option exists on
	//the command line
	//the : int he getopt indicates that the option preceeding the
	//: requires a argument to be specified
	while ((c = getopt(argc, argv, "c:b:a:sw:")) != -1) {
		switch (c) {
		case 'c':
			cache_capacity = atoi(optarg);
			c_flag = true;
			break;
		case 'b':
			cache_blocksize = atoi(optarg);
			b_flag = true;
			break;
		case 'a':
			cache_associativity = atoi(optarg);
			a_flag = true;
			break;
		case 's':
			split = true;
			break;
		case 'w':
			if (*optarg == 't' || *optarg == 'b')
				hwrite = *optarg;
			else if(*optarg == 'a' || *optarg == 'n')
				mwrite = *optarg;
			break;

		case ':':       /* -c without operand */
			fprintf(stderr,
				"Option -%c requires an operand\n", optopt);
			errflg =  true;
			break;
		case '?':
			fprintf(stderr, "Unrecognised option: -%c\n", optopt);
			errflg = true;
		}
	}

	//check if we have all the options and have no illegal options
	if (errflg || !c_flag || !b_flag || !a_flag) {
		fprintf(stderr, "usage: %s -c<capacity> -b<blocksize> -a<associativity>\n", argv[0]);
		system("pause");
		return false;
	}

	return true;

}

