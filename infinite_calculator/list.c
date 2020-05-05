#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"


/* DIGIT LIST FUNCTIONS */

/* constructor */
digit_list *make_digit_node(int digit)
{
    digit_list *ptr = (digit_list *)malloc(sizeof(digit_list));
    assert(ptr != NULL);

    ptr->digit = digit;
    ptr->next = ptr->prev = ptr;
    return ptr;
}

/* allows easy appending to end of a digit list */
digit_list *append_digit_list(digit_list *start, int digit)
{
    if (start == NULL) {
        /* special case for the first element */
        return make_digit_node(digit);
    }

    digit_list *new = make_digit_node(digit);
    digit_list *end = start->prev;
    end->next = new;
    new->prev = end;
    new->next = start;
    start->prev = new;
    return start;
}

/* show a digit list on its own line surrounded by brackets */
void show_digit_list(digit_list * start, FILE * stream)
{
    fprintf( stream, "[" );
    show_number(start, stream);
    fprintf( stream, "]\n" );
}

/* display a digit list inline as just a number */
void show_number(digit_list * start, FILE * stream)
{
    digit_list *ptr = start;
    while (ptr->next != start) {
        fprintf(stream, "%d", ptr->digit);
        ptr = ptr->next;
    }
    fprintf(stream, "%d", ptr->digit);
}

/* destructor */
void free_digit_list(digit_list * start)
{
    if (start == NULL) {
        return;
    }

    /* this code should work if I understand it right but it doesn't */
    /*
     * while (start != NULL) {
     *  digit_list *new_start = start->next;
     *  free(start)
     *  start = new_start
     * }
     */

    digit_list *head = start;

    while (start != NULL) {
        digit_list *new_start = start->next;
        free(start);
        start = new_start;
        if (start == head) {
            free(head);
            break;
        }
    }
}


/* NUMBER LIST FUNCTIONS */

/* constructor */
number_list *make_number_node(digit_list * d_list)
{
    number_list *ptr = (number_list *)malloc(sizeof(number_list));
    assert(ptr != NULL);

    ptr->number = d_list;
    ptr->next = ptr->prev = ptr;
    return ptr;
}

/* allows easy appending to end of a number list */
number_list *append_number_list(number_list *start, digit_list *number)
{
    if (start == NULL) {
        /* special case for the first element */
        return make_number_node(number);
    }

    number_list *new = make_number_node(number);
    number_list *end = start->prev;
    end->next = new;
    new->prev = end;
    new->next = start;
    start->prev = new;
    return start;
}

/* destructor */
void free_number_list(number_list * start)
{
    if (start == NULL) {
        return;
    }

    number_list *head = start;

    while (start != NULL) {
        number_list *new_start = start->next;
        free_digit_list(start->number);
        start = new_start;
        if (start == head) {
            free_digit_list(head->number);
            break;
        }
    }
}
