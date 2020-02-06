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
#include <assert.h>
#include "list.h"

#define BUFFER_SIZE		2

/* prototypes */
void formatter( FILE *, FILE *, int );
char *getword ( FILE * );


/* main driver */
int main( int argc, char *argv[] )
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

/* takes an input file and formats an output file to be right
 * aligned to the given width.
 */
void formatter( FILE *ifp, FILE *ofp, int width )
{
	char *str;
	list_t *start, *current;

	start = NULL;
	/* read a file and populate a linked list */
	while ( !feof( ifp ) ) {
		str = getword( ifp );
//#ifdef DEBUG
		fprintf( stderr, "[%s]\n", str );
//#endif
#ifdef LIST
		if ( start != NULL ) {
			current->next = make_node( str );
			current = current->next;
		}
		else {
			start = make_node( str );
			current = start;
		}
#endif
		free( str );
	}	
}

/* getword:
 * fetches the next word from the stream.
 */
char *getword( FILE *fp )
{
	int c;
	char *buffer;
	int i, size;

	/* request a character buffer of size BUFFER_SIZE */
	buffer = (char *)malloc( BUFFER_SIZE * sizeof(char) );
	assert( buffer != NULL );

	i = 0;
	size = BUFFER_SIZE;
	while ( !isspace(c = fgetc( fp )) ) {
		if ( c == EOF ) break;
#ifdef DEBUG
		fprintf( stderr, "%c", c );
#endif
		if ( i >= size-1 ) {
			buffer = (char *)realloc( buffer, size = 2*size );
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
#ifdef DEBUG
	fprintf( stderr, "{%d:%d:%s}\n", i, size, buffer );
#endif

	return buffer;		
}




