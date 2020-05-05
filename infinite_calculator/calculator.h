#ifndef CALCULATOR_H
#define CALCULATOR_H

# include "list.h"

/***
 * This is a structure for storing the data relevant to the calculator and implenting it's functions.
 */

/* constants */
#define END_OF_LINE		            257
#define END_OF_FILE                 258
#define NUMBER		                259


typedef struct calc {
    int current_token;
    digit_list *current_attribute;
    FILE * ifp;
} calculator;

calculator *create_calculator(FILE * );

digit_list *expr(calculator * );
digit_list *term(calculator * );
digit_list *factor(calculator * );
void match(calculator * , int);

digit_list *add(digit_list * , digit_list * );
digit_list *multiply(digit_list * , digit_list * );

int get_token(calculator * );
void error(char * );


#endif