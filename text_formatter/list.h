#ifndef LIST_H
#define LIST_H

typedef struct list_s {
    char *word;
    struct list_s *next;
}
        list_t;

list_t *make_node( char * );
void free_node( list_t * );

void show( list_t * );

#endif
