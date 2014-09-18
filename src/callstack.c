/*
 * callstack.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Callstack
 */

#include "callstack.h"

static int g_stack_top = 0;
static int *g_call_stack = NULL;
static int g_stack_capacity = CALLSTACK_BUCKET_SIZE;

static void enlarge_callstack()
{
	g_stack_capacity += CALLSTACK_BUCKET_SIZE;
	g_call_stack = realloc(g_call_stack, sizeof(int) * g_stack_capacity);
}

void callstack_init()
{
	g_stack_top = 0;
	g_stack_capacity = CALLSTACK_BUCKET_SIZE;
	g_call_stack = malloc(sizeof(int) * g_stack_capacity);
}

void callstack_destroy()
{
	g_stack_top = -1;
	g_stack_capacity = -1;

	free(g_call_stack);
}

int callstack_return()
{
	return g_stack_top == 0 ? -1 : g_call_stack[--g_stack_top];
}

void callstack_invoke(int ip)
{
	if (g_stack_top >= g_stack_capacity) enlarge_callstack();

	g_call_stack[g_stack_top++] = ip;
}
