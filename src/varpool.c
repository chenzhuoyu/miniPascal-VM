/*
 * varpool.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Variable Pool
 */

#include "varpool.h"

static var_node_t *find_id(list_t *pool, int id)
{
	var_node_t *node = (var_node_t *)pool->head;

	while (node && (node->id != id)) node = (var_node_t *)node->node.next;

	return node;
}

static var_node_t *get_node(list_t *pool, int id)
{
	var_node_t *node = find_id(pool, id);

	if (node == NULL)
	{
		node = malloc(sizeof(var_node_t));
		node->id = id;

		var_stack_init(&node->stack);
		list_append((list_t *)pool, (node_t *)node);
	}

	return node;
}

void pool_init(list_t *pool)
{
	pool->count = 0;
	pool->head = pool->tail = NULL;
}

void pool_destroy(list_t *pool)
{
	var_node_t *node = (var_node_t *)pool->head;

	while (node)
	{
		var_node_t *next = (var_node_t *)node->node.next;

		var_stack_destroy(&node->stack);
		free(node);

		node = next;
	}
}

void pool_undef(list_t *pool, int id)
{
	var_node_t *node = find_id(pool, id);

	if (node)
	{
		var_stack_discard_top(&node->stack);

		if (node->stack.usage == 0)
		{
			var_stack_destroy(&node->stack);
			list_remove((list_t *)pool, (node_t *)node);

			free(node);
		}
	}
}

variable_t *pool_reference(list_t *pool, int id)
{
	var_node_t *node = find_id(pool, id);
	return node ? var_stack_top(&node->stack) : NULL;
}

variable_t *pool_def_integer(list_t *pool, int id)
{
	return var_stack_push_integer(&get_node(pool, id)->stack, 0);
}

variable_t *pool_def_real(list_t *pool, int id)
{
	return var_stack_push_real(&get_node(pool, id)->stack, 0.0f);
}

variable_t *pool_def_char(list_t *pool, int id)
{
	return var_stack_push_char(&get_node(pool, id)->stack, '\0');
}

variable_t *pool_def_string(list_t *pool, int id)
{
	return var_stack_push_string(&get_node(pool, id)->stack, NULL);
}

variable_t *pool_def_boolean(list_t *pool, int id)
{
	return var_stack_push_boolean(&get_node(pool, id)->stack, 0);
}
