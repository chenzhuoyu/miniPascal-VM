/*
 * mempool.c
 *
 *  Created on: 2013-2-8
 *      Author: desperedo
 *
 *	miniPascal VM Memory Pool
 */

#include "mempool.h"

static mem_pool_t g_mem_pool;

static void new_bucket()
{
	mem_bucket_t *bucket = malloc(sizeof(mem_bucket_t));

	list_append((list_t *)&g_mem_pool.pool, (node_t *)bucket);
	bucket->pool = malloc(sizeof(variable_t) * MEM_POOL_BUCKET_SIZE);

	for (int i = 0; i < MEM_POOL_BUCKET_SIZE; i++) list_append((list_t *)&g_mem_pool.free, (node_t *)&bucket->pool[i]);
}

void mem_init()
{
	g_mem_pool.usage = 0;
	g_mem_pool.capacity = 0;

	list_init((list_t *)&g_mem_pool.pool);
	list_init((list_t *)&g_mem_pool.free);
	list_init((list_t *)&g_mem_pool.using);

	new_bucket();
}

void mem_destroy()
{
	node_t *node = g_mem_pool.pool.head;

	while (node)
	{
		node_t *next = node->next;
		mem_bucket_t *bucket = (mem_bucket_t *)node;

		free(bucket->pool);
		free(bucket);

		node = next;
	}
}

variable_t *mem_alloc()
{
	if (g_mem_pool.free.count == 0) new_bucket();

	node_t *node = g_mem_pool.free.head;

	g_mem_pool.usage++;

	list_remove((list_t *)&g_mem_pool.free,  node);
	list_append((list_t *)&g_mem_pool.using, node);

	return (variable_t *)node;
}

void mem_free(variable_t *ptr)
{
	g_mem_pool.usage--;

	list_remove((list_t *)&g_mem_pool.using, (node_t *)ptr);
	list_append((list_t *)&g_mem_pool.free,  (node_t *)ptr);
}
