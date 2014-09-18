/*
 * varstack.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM VariableStack
 */

#ifndef __VARSTACK_H__
#define __VARSTACK_H__

#include "global.h"
#include "variable.h"

#define STACK_BUCKET_SIZE		256

typedef struct var_stack_t
{
	int			usage;
	int			capacity;
	variable_t	**stack;
} var_stack_t;

void var_stack_init(var_stack_t *stack);
void var_stack_destroy(var_stack_t *stack);

void var_stack_discard_top(var_stack_t *stack);
void var_stack_duplicate_top(var_stack_t *stack);

variable_t *var_stack_top(var_stack_t *stack);
variable_t *var_stack_reference(var_stack_t *stack, int index);

variable_t *var_stack_pop(var_stack_t *stack);
void var_stack_push(var_stack_t *stack, variable_t *value);
variable_t *var_stack_push_integer(var_stack_t *stack, int value);
variable_t *var_stack_push_real(var_stack_t *stack, double value);
variable_t *var_stack_push_char(var_stack_t *stack, char value);
variable_t *var_stack_push_string(var_stack_t *stack, char *value);
variable_t *var_stack_push_boolean(var_stack_t *stack, bool value);

#endif /* __VARSTACK_H__ */
