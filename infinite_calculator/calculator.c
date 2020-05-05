#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "calculator.h"

/* initializes a calculator object */
calculator *create_calculator(FILE *in)
{
    calculator *ptr = (calculator *)malloc(sizeof(calculator));
    assert(ptr != NULL);

    ptr->ifp = in;
    return ptr;
}


/* CALCULATOR */

/* handles addition */
digit_list *expr(calculator *C)
{
    /* check first for multiplication */
    digit_list *value = term(C);
    /* keep adding so long as the current token is a + sign */
    while (1) {
        if (C->current_token == '+') {
            match(C, '+');
            value = add(value, term(C));
        }
        else {
            break;
        }
    }
    return value;
}

/* handles mulitiplication */
digit_list *term(calculator *C)
{
    /* check first for parentheses and numbers */
    digit_list  *value = factor(C);
    /* keep multiplying so long as the current token is a * sign */
    while (1) {
        if (C->current_token == '*') {
            match(C, '*');
            value = multiply(value, factor(C));
        }
        else {
            break;
        }
    }
    return value;
}

/* handles parentheses and numbers */
digit_list *factor(calculator *C)
{
    digit_list *value = NULL;
    /* if there are parentheses, evaluate whatever is inside them */
    if (C->current_token == '(') {
        match(C, '(');
        free_digit_list(value);
        value = expr(C);
        match(C, ')');
    }
    /* otherwise, check if the current token is a number */
    else if (C->current_token == NUMBER) {
        free_digit_list(value);
        value = C->current_attribute;
        match(C, NUMBER);
    }
    else if (C->current_token == END_OF_FILE) {
        /* the program has finished processing and there is nothing left to do */
        exit(0);
    }
    else {
        error("Unexpected token in factor()");
    }

    /* won't ever be NULL, the error would have triggered and stopped the program */
    return value;
}

/* checks if the current token is what is expected, and if it is then it fetches the next */
void match(calculator *C, int expected_token)
{
    if (C->current_token == expected_token) {
        C->current_token = get_token(C);
    }
    else {
        error("Unexpected token in match");
    }
}


/* OPERATIONS */

/* performs addition on two digit lists and returns their sum */
digit_list *add(digit_list *left, digit_list *right)
{
    digit_list *cdl = left->prev;      // current dight left
    digit_list *cdr = right->prev;     // current digit right
    digit_list *cd_result = NULL;      // current digit result
    digit_list *ldr = NULL;            // last digit result

    int digit;
    int carry;
    int last_carry = 0;

    /* continue looping until both arguments have been fully processed */
    while (!(cdl == NULL && cdr == NULL)) {
        /* get sum */
        if (cdl != NULL && cdr != NULL) {
            digit = cdl->digit + cdr->digit + last_carry;
        }
        else if (cdl != NULL) {
            digit = cdl->digit + last_carry;
        }
        else {
            digit = cdr->digit + last_carry;
        }

        /* calculate digit and carry */
        carry = digit / 10;
        digit %= 10;

        /* create and link nodes */
        if (cd_result == NULL) {
            cd_result = make_digit_node(digit);
            ldr = cd_result;
        }
        else {
            cd_result->prev = make_digit_node(digit);
            cd_result->prev->next = cd_result;
            cd_result = cd_result->prev;
        }

        /* update pointers for current digits and the carry for the last digit */
        if (cdl != left && cdl != NULL) {
            cdl = cdl->prev;
        }
        else if (cdl == left) {
            cdl = NULL;
        }

        if (cdr != right && cdr != NULL) {
            cdr = cdr->prev;
        }
        else if (cdr == right) {
            cdr = NULL;
        }
        last_carry = carry;

        /* special case for when the carry would create a new digit */
        if (cdl == NULL && cdr == NULL && last_carry == 1) {
            cd_result->prev = make_digit_node(last_carry);
            cd_result->prev->next = cd_result;
            cd_result = cd_result->prev;
            ldr->next = cd_result;
            cd_result->prev = ldr;
        }
    }

    /* circularly link the list of digits */
    cd_result->prev = ldr;
    ldr->next = cd_result;

    /* delete the left list, by convention it will be overwritten */
    // Nothing I do here works properly

    return cd_result;
}

/* performs long multiplication to produce a set of digit lists and then adds them together. */
digit_list *multiply(digit_list *left, digit_list *right)
{
    number_list *result_list = NULL;
    digit_list *final_result = make_digit_node(0);
    int zero_counter = 0;

    int digit;
    int carry;

    digit_list *cdl = left->prev;      // current dight left
    digit_list *cdr = right->prev;     // current digit right

    /* for every digit of the right argument, create a new number by multiplying it with the entire left argument.
     * Then, append the correct amount of zeros to the produced number and store it in a list.
     * At the end, sum the entire list for the final product. */
    while (cdr != NULL) {

        digit_list *cd_result = NULL;      // current digit result
        digit_list *ldr = NULL;            // last digit result
        int last_carry = 0;

        /* internal loop, each run of this will produce one sub-product. */
        while (cdl != NULL) {
            /* get product */
            digit = cdl->digit * cdr->digit;

            /* calculate digit and carry */
            carry = digit / 10;
            digit %= 10;

            /* create and link nodes */
            if (cd_result == NULL) {
                /* special case for the first digit of a number */
                cd_result = make_digit_node(digit);
                ldr = cd_result;
            }
            else {
                /* there may be an extra carry operation that needs to be performed */
                carry += (digit + last_carry) / 10;
                digit = (digit + last_carry) % 10;

                cd_result->prev = make_digit_node(digit);
                cd_result->prev->next = cd_result;
                cd_result = cd_result->prev;
            }

            /* update pointers for current digits and the carry for the last digit */
            if (cdl != left) {
                cdl = cdl->prev;
            }
            else if (cdl == left) {
                cdl = NULL;
            }
            last_carry = carry;

            /* special case for when the carry would create a new digit */
            if (cdl == NULL && last_carry != 0) {
                cd_result->prev = make_digit_node(last_carry);
                cd_result->prev->next = cd_result;
                cd_result = cd_result->prev;
                ldr->next = cd_result;
                cd_result->prev = ldr;
            }

            /* the number is done and it needs to be linked circularly */
            else {
                cd_result->prev = ldr;
                ldr->next = cd_result;
            }
        }

        /* append this result to the list of sub-products */
        if (ldr != NULL) {
            /* appends one zero to the subproduct for every digit to the left cdr is */
            int i;
            for (i = 0; i < zero_counter; ++i) {
                append_digit_list(cd_result, 0);
            }

            result_list = append_number_list(result_list, cd_result);
        }

        /* update pointers */
        cdl = left->prev;
        if (cdr != right) {
            cdr = cdr->prev;
        }
        else if (cdr == right) {
            cdr = NULL;
        }

        /* update zero counter */
        zero_counter += 1;
    }

    /* add all of the subproducts together into the final result */
    number_list *ptr = result_list;
    assert(ptr != NULL);
    while (ptr->next != result_list) {
        final_result = add(final_result, ptr->number);
        ptr = ptr->next;
    }
    final_result = add(final_result, ptr->number);

    /* deallocate results list */
    // Nothing I do here works properly

    /* return final result */
    return final_result;
}


/* I/O */

/* fetches the next token from the stream and stores it as an attribute */
int get_token(calculator *C)
{
    int character;
    digit_list *number;

    while (1) {
        switch (character = fgetc(C->ifp)) {
            case '+': case '*': case '(': case ')':
                /* valid operators and parentheses are returned as is */
                return character;
            case ' ': case '\t':
                /* completely ignore spaces and tabs */
                continue;
            case '\n':
                /* return a special value signifying that get_token found an end of line */
                return END_OF_LINE;
            default:
                /* the token is likely a number */
                if (isdigit(character)) {
                    /* create a digit list and append digits to it until a non-digit character is found */
                    number = make_digit_node(character - '0');
                    while (isdigit(character = fgetc(C->ifp))) {
                        number = append_digit_list(number, character - '0');
                    }
                    /* put the non-digit character back */
                    ungetc(character, C->ifp);
                    /* the calculator what the current number is */
                    // It makes sense I should be able to free the previous current attribute, but it breaks if I do
                    // free_digit_list(C->current_attribute);
                    C->current_attribute = number;
                    /* return a special value signifying that get_token found a number */
                    return NUMBER;
                }
                else if (feof(C->ifp)) {
                        /* return a special value signifying that the end of file was found */
                        return END_OF_FILE;
                } else {
                    fprintf(stderr, "{%c} ", character);
                    error("Unknown token in get_token()");
                }
        }
    }
}

/* reports to the user if an error has been encountered */
void error(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}
