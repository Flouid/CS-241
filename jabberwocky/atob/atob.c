#include <stdio.h>
#include <stdlib.h>

/* converts a text file of ascii characters to binary */
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

    /* iterate through every character in the input file and parse it into binary */
    int character = fgetc(ifp);
    while (!feof(ifp)) {
        int exponent;
        for (exponent = 7; exponent >= 0; --exponent) {
            if (character - (1 << exponent) >= 0) {
                fprintf(ofp, "1");
                character -= (1 << exponent);
            }
            else {
                fprintf(ofp, "0");
            }
        }
        character = fgetc(ifp);
    }
    fclose(ifp);
    fclose(ofp);
}