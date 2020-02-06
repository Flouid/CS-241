#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

/* constructor */
list_t *make_node( char *str )
{
	list_t *ptr;

	ptr = (list_t *)malloc( sizeof(list_t) );
	assert( ptr != NULL );
	ptr->word = (char *)strdup( str );
	ptr->next = NULL;

	return ptr;
}

/* destructor */
void free_node( list_t *ptr )
{
	free( ptr );
}

/* show linked list */
void show( list_t *start )
{
	list_t *ptr;

	for ( ptr = start; ptr != NULL; ptr = ptr->next ) {
		fprintf( stderr, "[%s]\n", ptr->word );
	}
}










