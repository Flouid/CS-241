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
#include <string.h>
#include <ctype.h>

#include "list.h"

#define BUFFER_SIZE     2

/* prototypes */
void formatter( FILE *, FILE *, int );
char *getword( FILE * );
char *get_new_buffer( int );

int main( int argc, char *argv[] )
{
    FILE *ifp, *ofp;
    clock_t begin_time, end_time;
    double time_used;


    if ( argc < 4 ) {
        fprintf( stderr,"Not enough arguments\n" );
        exit( 1 );
    }
    if ( !( ifp = fopen( argv[1],"r" ) ) ) {
        fprintf( stderr,"Cannot open file %s\n",argv[1] );
        exit( 1 );
    }
    if ( !( ofp = fopen( argv[2],"w" ) ) ) {
        fprintf( stderr,"Cannot open file %s\n",argv[2] );
        exit( 1 );
    }

    begin_time = clock();
    formatter( ifp, ofp, atoi( argv[3] ) );
    end_time = clock();
    time_used = (double)(end_time - begin_time)/CLOCKS_PER_SEC;
    fprintf( stderr, "Time usage = %17.13f\n", time_used );

    fclose( ifp );
    fclose( ofp );

    return 0;
}

/* takes an input file and formats an output file to be right
 * aligned to the given width.
 */
void formatter( FILE *ifp, FILE *ofp, int width )
{
    char *str;
    list_t *start = NULL, *current = NULL;

    /* read a file and populate a linked list. */
    while ( !feof( ifp ) ) {
        str = getword( ifp );
        if ( strlen(str) > width - 1 ) {
            fprintf( stderr, "Word too long to fit on a single line: [%s]\n", str );
            return;
        }
        if ( start != NULL ) {
            current->next = make_node( str );
            current = current->next;
        }
        else {
            start = make_node( str );
            current = start;
        }
        free( str );
    }

    int length = 0, word_count = 0;
    char *buffer = get_new_buffer( width );
    list_t *ptr;
    list_t *line_start = current = start;

    /* The last character of the buffer should be a terminating character that never changes. */
    buffer[width] = '\0';

    /* iterate through the linked list creating lines and writing them to the output file. */
    while ( current != NULL ) {
        int word_length = strlen( current->word );

        /* Calculate the length of the line and number of words. */
        if ( length + word_count + word_length <= width ) {
            length += word_length;
            if ( word_length > 0 ) {
                word_count++;
            }
        }

        /* Create the line and write it. */
        else {
            int spacing;
            int remainder;
            if ( word_count > 1 ) {
                spacing = (width - length) / (word_count - 1);
                remainder = (width - length) % (word_count - 1);
            }
            else {
                spacing = 0;
                remainder = width - length;
            }

            /* Iterate through every word on the line and populate the buffer. */
            int buffer_index = 0;
            for ( ptr = line_start; ptr != current; ptr = ptr->next ) {
                word_length = strlen( ptr->word );
                if ( word_length == 0 ) {
                    continue;
                }

                /* Iterate through the word and add it to the buffer. */
                int index;
                int word_index = 0;
                for ( index = buffer_index; index < word_length + buffer_index; ++index ) {
                    buffer[index] = ptr->word[word_index];
                    ++word_index;
                }
                buffer_index += word_length;

                /* Add the correct number of spaces after the word */
                if ( buffer_index < width ) {
                    for ( index = 0; index < spacing; ++index ) {
                        buffer[buffer_index] = ' ';
                        ++buffer_index;
                    }
                }

                /* if there is a remainder, add a single space after the word. */
                if ( remainder > 0 ) {
                    buffer[buffer_index] = ' ';
                    ++buffer_index;
                    --remainder;
                }
            }

            /* Populate all unused characters in the buffer with spaces. */
            while ( buffer_index < width ) {
                buffer[buffer_index] = ' ';
                ++buffer_index;
            }

            fprintf( ofp, "%s\n", buffer );

            length = word_count = 0;
            line_start = current;

            /* Line is complete. */
            continue;
        }

        /* Special case for the very last line. */
        if ( current->next == NULL ) {

            /* Populate the last line of the buffer. */
            int buffer_index = 0;
            for ( ptr = line_start; ptr != current; ptr = ptr->next ) {
                word_length = strlen( ptr->word );
                if ( word_length == 0 ) {
                    continue;
                }

                /* Iterate through the word and add it to the buffer. */
                int index;
                int word_index = 0;
                for ( index = buffer_index; index < word_length + buffer_index; ++index ) {
                    buffer[index] = ptr->word[word_index];
                    ++word_index;
                }
                buffer_index += word_length;

                /* Add the correct number of spaces after the word */
                if ( ptr->next != current ) {
                    buffer[buffer_index] = ' ';
                    ++buffer_index;
                }
            }
            fprintf( ofp, "%s\n", buffer );
        }
        current = current->next;
    }
    free ( buffer );
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
    buffer = get_new_buffer(BUFFER_SIZE);

    i = 0;
    size = BUFFER_SIZE;
    while ( !isspace( c = fgetc( fp ) ) ) {
        if ( c == EOF ) break;
        if ( i >= size-1 ) {
            buffer = (char *)realloc( buffer, size = 2*size );
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    return buffer;
}

/*
 * returns a pointer to a character array of the given size of characters plus '\0'.
 */
inline char *get_new_buffer( int size )
{
    char *buffer = (char *)malloc( (size + 1) * sizeof(char) );
    assert ( buffer != NULL );
    return buffer;
}



