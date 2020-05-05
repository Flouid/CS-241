#ifndef LIST_H
#define LIST_H

/***
 * This is a set of structures for storing the numbers produced by this calculator.
 * Because of the unbounded memory requirement, numbers are stored as a linked list of digits.
 * A seperate type of linked list is defined for storing a list of number lists.
 * This is done so that multiplication can be implemented in such a way that produces subproducts which can be summed.
 */

typedef struct list_a {
    int digit;
    struct list_a *next;
    struct list_a *prev;
} digit_list;

digit_list *make_digit_node(int);
digit_list *append_digit_list(digit_list * , int);

void show_digit_list(digit_list * , FILE * );
void show_number(digit_list * , FILE * );

void free_digit_list(digit_list * );



typedef struct list_b {
    digit_list *number;
    struct list_b *next;
    struct list_b *prev;
} number_list;

number_list *make_number_node(digit_list * );
number_list *append_number_list(number_list * , digit_list * );

void free_number_list(number_list * );


#endif