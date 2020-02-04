/*---------------------------------------------------------------------- 
 * formatter.c:
 *
 * compilation: 
 *	gcc -o formatter formatter.c
 * usage:
 *	./formatter <input-file> <output-file> <width>
 * 
 *-----------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void formatter( FILE *, FILE *, int );

main( int argc, char *argv[] )
{
	FILE *ifp, *ofp;
	clock_t begin_time, end_time;
	double time_used;


	if (argc < 4) {
	  fprintf(stderr,"Not enough arguments\n");
	  exit(1);
	}
	if (!(ifp = fopen(argv[1],"r"))) {
	  fprintf(stderr,"Cannot open file %s\n",argv[1]);
	  exit(1);
	}
	if (!(ofp = fopen(argv[2],"w"))) {
	  fprintf(stderr,"Cannot open file %s\n",argv[2]);
	  exit(1);
	}

	begin_time = clock();
	formatter( ifp, ofp, atoi(argv[3]));
	end_time = clock();
	time_used = (double)(end_time - begin_time)/CLOCKS_PER_SEC;
	fprintf( stderr, "Time usage = %17.13f\n", time_used );

  	fclose(ifp);
  	fclose(ofp);
}

void formatter( FILE *ifp, FILE *ofp, int width )
{}


