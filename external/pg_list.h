#ifndef PG_LIST_H
#define PG_LIST_H

#include "nodes.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct ListCell ListCell;

typedef struct List
	: public Node /* T_List, T_IntList, or T_OidList */
{
	// inherited location field is not currently used in lists
	typedef List This;
	int length;
	ListCell *head;
	ListCell *tail;
} List;

struct ListCell
{
	union
	{
		void	   *ptr_value;
		int			int_value;
	}			data;
	ListCell   *next;
};

/*
 * The *only* valid representation of an empty list is NIL; in other
 * words, a non-NIL list is guaranteed to have length >= 1 and
 * head/tail != NULL
 */
#define NIL						((List *) NULL)

static inline const ListCell *
list_head(const List * const l)
{
	return l ? l->head : NULL;
}

static inline ListCell *
list_head(List *l)
{
	return l ? l->head : NULL;
}

static inline const ListCell *
list_tail(const List * const l)
{
	return l ? l->tail : NULL;
}

static inline ListCell *
list_tail(List *l)
{
	return l ? l->tail : NULL;
}

static inline int
list_length(const List * const l)
{
	return l ? l->length : 0;
}

#endif   /* PG_LIST_H */
