/*
 * list.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	two-way linked list
 */

#ifndef __LIST_H__
#define __LIST_H__

#include "global.h"

typedef struct _node_t
{
	struct _node_t	*prev;
	struct _node_t	*next;
} node_t;

typedef struct _list_t
{
	ulong	count;
	node_t	*head;
	node_t	*tail;
} list_t;

static inline void list_init(list_t *list)
{
	list->count = 0;
	list->tail = list->head = NULL;
}

static inline void list_remove(list_t *list, node_t *node)
{
	if (list->count)
	{
		list->count--;

		if (node == list->tail) list->tail = list->tail->prev;
		if (node == list->head) list->head = list->head->next;

		if (node->next) node->next->prev = node->prev;
		if (node->prev) node->prev->next = node->next;

		node->prev = node->next = NULL;
	}
}

static inline void list_append(list_t *list, node_t *node)
{
	list->count++;

	if (list->head == NULL)
	{
		node->prev = node->next = NULL;
		list->tail = list->head = node;
	}
	else
	{
		node->next = NULL;
		node->prev = list->tail;

		list->tail->next = node;
		list->tail = node;
	}
}

#endif /* __LIST_H__ */
