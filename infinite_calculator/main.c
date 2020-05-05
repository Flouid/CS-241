#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"

/* unbounded memory calculator */
int main( int argc, char *argv[] )
{
    FILE *ifp, *ofp;

    /* making sure program has valid input arguments */
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

    //FILE *stream = stderr;      // console output
    FILE *stream = ofp;         // file output

    /* all of the variables necessary to run the calculator are wrapped into one data type */
    calculator *C = create_calculator(ifp);
    digit_list *value = NULL;

    /* outer loop runs once for every line */
    while(1) {
        C->current_token = get_token(C);
        /* inner loop runs for every term */
        while (C->current_token != END_OF_LINE) {
            value = expr(C);
            show_number(value, stream);
            fprintf(stream, "\n");
        }
    }
}
