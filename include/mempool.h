/*
 * mempool.h
 *
 *  Created on: 2013-2-8
 *      Author: desperedo
 *
 *	miniPascal VM Memory Pool
 */

#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include "global.h"

#include "list.h"
#include "variable.h"

#define MEM_POOL_BUCKET_SIZE		4096

typedef struct _mem_bucket_t
{
	node_t		node;
	variable_t	*pool;
} mem_bucket_t;

typedef struct _mem_pool_t
{
	int			usage;
	int			capacity;

	list_t		free;
	list_t		using;
	list_t		pool;
} mem_pool_t;

void mem_init();
void mem_destroy();

variable_t *mem_alloc();
void mem_free(variable_t *ptr);

#endif /* __MEMPOOL_H__ */
