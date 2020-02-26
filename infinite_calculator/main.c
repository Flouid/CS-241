#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Constants
#define EOS		257
#define NUM		258

// Global communication variables
int current_token;
int current_attribute;

// Prototypes
int expr();
int term();
int factor();
void error ( char * );
int get_token();
void match( int expected_token );

/* bounded memory calculator */
int main( int argc, char *argv[] )
{
    FILE *ifp, *ofp;
    int value;

    if (argc < 3) {
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

    current_token = get_token();
    while ( current_token != EOS ) {
        value = expr();
        fprintf( stderr, "\nValue = %d\n", value );
    }
}

/* calculator */

// handles addition
int expr()
{
    int value = term();
    while (1) {
        if ( current_token == '+' ) {
            match( '+' );
            value += term();
        }
        else break;
    }
    return value;
}

// handles multiplication
int term()
{
    int value = factor();
    while (1) {
        if ( current_token == '*' ) {
            match( '*' );
            value *= factor();
        }
        else break;
    }
    return value;
}

// handles brackets and numbers
int factor()
{
    int value;

    if ( current_token == '(' ) {
        match( '(' );
        value = expr();
        match( ')' );
    }
    else if ( current_token == NUM ) {
        value = current_attribute;
        match ( NUM );
        return value;
    }
    else error( "Unexpected token in factor()" );
}

void match( int expected_token )
{
    if ( current_token == expected_token ) {
        current_token = get_token();
    }
    else {
        error( "Unexpected token in match" );
    }
}

/* get next token */
int get_token()
{
    int c;
    int value;

    while (1) {
        switch ( c = getchar() ) {
            case '+': case '-': case '*': case '(': case ')':
                return c;	// return operators and brackets as is
            case ' ': case '\t':
                continue;	// ignore spaces and tabs
            default:
                if ( isdigit(c) ) {
                    value = c - '0';
                    while ( isdigit( c = getchar() )) {
                        value = value * 10 + (c - '0');
                    }
                    ungetc( c, stdin );
                    current_attribute = value;
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

/* error reporting function */
void error( char *message )
{
    fprintf( stderr, "Error: %s\n", message );
    exit(1);
}