#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

// Constants
#define EOS		257
#define NUM		258

// Prototypes
void error ( char * );
int get_token() ;

/* bounded memory calculator */
int main()
{
	int current_token;

	while ( current_token != EOS ) {
		current_token = get_token();
	}
}

int get_token()
{
	int c;
	int value;

	while (1) {
		switch ( c = getchar() ) {
		case '+': case '-': case '*': case '(': case ')':
			fprintf( stderr, "[OP:%c]", c );
			return c;	// return operators and brackets as is
		case ' ': case '\t':
			continue;	// ignore spaces and tabs
		default:
			if ( isdigit(c) ) {
				fprintf( stderr, "[%c]", c );
				while ( isdigit( c = getchar() )) {
					fprintf( stderr, "[%c]", c );
				}
				ungetc( c, stdin );
				return NUM;
			}
			else if (c == '\n') {
				return EOS;
			}
			else {
				fprintf( stderr, "{%c} ", c );
				error( "Unknown token" );
			}
		}
	}
}

void error( char *message )
{
	fprintf( stderr, "Error: %s\n", message );
	exit(1);
}
