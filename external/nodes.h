#ifndef NODES_H
#define NODES_H

#include "node_tags.h"

typedef enum NodeTag NodeTag;

/*
 * The first field of every node is NodeTag. Each node created (with makeNode)
 * will have one of the following tags as the value of its first field.
 *
 * Note that the numbers of the node tags are not contiguous. We left holes
 * here so that we can add more tags without changing the existing enum's.
 * (Since node tag numbers never exist outside backend memory, there's no
 * real harm in renumbering, it just costs a full rebuild ...)
 */

/*
 * The first field of a node of any type is guaranteed to be the NodeTag.
 * Hence the type of any node can be gotten by casting it to Node. Declaring
 * a variable to be of Node * (instead of void *) can also facilitate
 * debugging.
 */

typedef struct Node
{
	virtual ~Node()
	{
	}
	NodeTag		type;
} Node;

#define nodeTag(nodeptr)		(((Node*)(nodeptr))->type)

/*
 * Ident - specifies a reference to a namespace, variable, property, function name etc.
 */
typedef struct Ident
	: public Node
{
	typedef Ident This;
	const wchar_t* name;
} Ident;


#define IS_A(nodeptr,_type_)		(nodeTag(nodeptr) == T_##_type_)


#endif   /* NODES_H */
