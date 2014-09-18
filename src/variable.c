/*
 * variable.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Variables
 */

#include "mempool.h"
#include "variable.h"

void var_destroy(variable_t **var)
{
	switch ((*var)->type)
	{
		case VAR_TYPE_STRING: free((*var)->string); break;
		case VAR_TYPE_ARRAY:
		{
			for (int i = 0; i <= (*var)->high - (*var)->low; i++) var_destroy(&(*var)->data[i]);

			free((*var)->data);

			break;
		}
	}

	mem_free(*var);
	*var = NULL;
}

variable_t *var_clone(variable_t *var)
{
	variable_t *result = mem_alloc();

	result->length = -1;
	result->string = NULL;
	result->type = var->type;
	var_copy_value(result, var);

	return result;
}

variable_t *var_create_integer(int value)
{
	variable_t *result = mem_alloc();

	result->integer = value;
	result->type = VAR_TYPE_INTEGER;

	return result;
}

variable_t *var_create_real(double value)
{
	variable_t *result = mem_alloc();

	result->real = value;
	result->type = VAR_TYPE_REAL;

	return result;
}

variable_t *var_create_char(char value)
{
	variable_t *result = mem_alloc();

	result->character = value;
	result->type = VAR_TYPE_CHAR;

	return result;
}

variable_t *var_create_string(char *value)
{
	variable_t *result = mem_alloc();

	result->length = -1;
	result->string = NULL;
	result->type = VAR_TYPE_STRING;
	if (value) var_put_string(result, value);

	return result;
}

variable_t *var_create_boolean(bool value)
{
	variable_t *result = mem_alloc();

	result->type = VAR_TYPE_BOOLEAN;
	result->boolean = value ? 1 : 0;

	return result;
}

void var_put_string(variable_t *dest, char *value)
{
	int length = strlen(value);

	if (length > dest->length)
	{
		if (dest->string) free(dest->string);

		dest->length = length;
		dest->string = malloc(dest->length + 1);
	}

	strcpy(dest->string, value);
}

void var_make_array(variable_t *var, int low, int high)
{
	variable_t **buffer = malloc(sizeof(variable_t *) * (high - low + 1));

	for (int i = 0; i <= high - low; i++) buffer[i] = var_clone(var);

	if (var->type == VAR_TYPE_ARRAY) free(var->data);
	if (var->type == VAR_TYPE_STRING) free(var->string);

	var->low = low;
	var->high = high;
	var->data = buffer;
	var->type = VAR_TYPE_ARRAY;
}
