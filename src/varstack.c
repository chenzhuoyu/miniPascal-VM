/*
 * varstack.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Variable Stack
 */

#include "varstack.h"

static void enlarge_stack(var_stack_t *stack)
{
	stack->capacity += STACK_BUCKET_SIZE;
	stack->stack = realloc(stack->stack, sizeof(variable_t *) * stack->capacity);
}

void var_stack_init(var_stack_t *stack)
{
	stack->usage = 0;
	stack->capacity = STACK_BUCKET_SIZE;
	stack->stack = malloc(sizeof(variable_t *) * stack->capacity);
}

void var_stack_destroy(var_stack_t *stack)
{
	for (int i = 0; i < stack->usage; i++)
		var_destroy(&stack->stack[i]);
	free(stack->stack);
}

void var_stack_discard_top(var_stack_t *stack)
{
	variable_t *top = var_stack_pop(stack);

	var_destroy(&top);
}

void var_stack_duplicate_top(var_stack_t *stack)
{
	var_stack_push(stack, var_clone(var_stack_top(stack)));
}

variable_t *var_stack_top(var_stack_t *stack)
{
	return stack->stack[stack->usage - 1];
}

variable_t *var_stack_reference(var_stack_t *stack, int index)
{
	return (index < 0 || index >= stack->usage) ? NULL : stack->stack[stack->usage - index - 1];
}

variable_t *var_stack_pop(var_stack_t *stack)
{
	return stack->stack[--stack->usage];
}

void var_stack_push(var_stack_t *stack, variable_t *value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	stack->stack[stack->usage++] = value;
}

variable_t *var_stack_push_integer(var_stack_t *stack, int value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	return stack->stack[stack->usage++] = var_create_integer(value);
}

variable_t *var_stack_push_real(var_stack_t *stack, double value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	return stack->stack[stack->usage++] = var_create_real(value);
}

variable_t *var_stack_push_char(var_stack_t *stack, char value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	return stack->stack[stack->usage++] = var_create_char(value);
}

variable_t *var_stack_push_string(var_stack_t *stack, char *value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	return stack->stack[stack->usage++] = var_create_string(value);
}

variable_t *var_stack_push_boolean(var_stack_t *stack, bool value)
{
	if (stack->usage >= stack->capacity) enlarge_stack(stack);
	return stack->stack[stack->usage++] = var_create_boolean(value);
}
