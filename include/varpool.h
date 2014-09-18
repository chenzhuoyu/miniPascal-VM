/*
 * varpool.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Variable Pool
 */

#ifndef __VARPOOL_H__
#define __VARPOOL_H__

#include "global.h"

#include "list.h"
#include "varstack.h"
#include "variable.h"

typedef struct _var_node_t
{
	node_t				node;

	int					id;
	var_stack_t			stack;
} var_node_t;

void pool_init(list_t *pool);
void pool_destroy(list_t *pool);

void pool_undef(list_t *pool, int id);
variable_t *pool_reference(list_t *pool, int id);

variable_t *pool_def_integer(list_t *pool, int id);
variable_t *pool_def_real(list_t *pool, int id);
variable_t *pool_def_char(list_t *pool, int id);
variable_t *pool_def_string(list_t *pool, int id);
variable_t *pool_def_boolean(list_t *pool, int id);

#endif /* __VARPOOL_H__ */
