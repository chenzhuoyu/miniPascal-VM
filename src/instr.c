/*
 * instr.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Instructions
 */

#include "instr.h"

static bytecode_t *g_bytecode = NULL;

static list_t *g_var_pool = NULL;
static var_stack_t *g_vm_stack = NULL;

static variable_t *g_operand1 = NULL;
static variable_t *g_operand2 = NULL;

static variable_t *g_previous_var = NULL;

void instr_set_operands(variable_t *operand1, variable_t *operand2)
{
	g_operand1 = operand1;
	g_operand2 = operand2;
}

void instr_init(bytecode_t *bytecode, list_t *pool, var_stack_t *vmstack)
{
	g_var_pool = pool;
	g_vm_stack = vmstack;
	g_bytecode = bytecode;
}

bool INSTR_drop()
{
	var_stack_discard_top(g_vm_stack);
	return 1;
}

bool INSTR_dup()
{
	var_stack_duplicate_top(g_vm_stack);
	return 1;
}

bool INSTR_push()
{
	var_stack_push(g_vm_stack, var_clone(pool_reference(g_var_pool, g_operand1->integer)));
	return 1;
}

bool INSTR_pusha()
{
	bool result = 1;
	bool processed = 0;
	variable_t *array = pool_reference(g_var_pool, g_operand2->integer);
	uchar type = array->type;

	for (int i = 0; result && i < g_operand1->integer; i++)
	{
		variable_t *index = var_stack_pop(g_vm_stack);

		switch (type)
		{
			case VAR_TYPE_STRING:
			{
				if (index->integer >= 1 && index->integer <= array->length)
				{
					processed = 1;
					type = VAR_TYPE_CHAR;
					var_stack_push_char(g_vm_stack, array->string[index->integer - 1]);
				}
				else
				{
					result = 0;
					printf("error : string index out of bounds.\n");
				}

				break;
			}

			case VAR_TYPE_ARRAY:
			{
				if (index->integer >= array->low && index->integer <= array->high)
				{
					array = array->data[index->integer - array->low];
					type = array->type;
				}
				else
				{
					result = 0;
					printf("error : array index out of bounds.\n");
				}

				break;
			}

			default:
			{
				result = 0;
				printf("error : cannot index regular variables.\n");
			}
		}

		var_destroy(&index);
	}

	if (!processed) var_stack_push(g_vm_stack, var_clone(array));

	return result;
}

bool INSTR_pop()
{
	variable_t *variable = var_stack_pop(g_vm_stack);

	var_copy_value(pool_reference(g_var_pool, g_operand1->integer), variable);
	var_destroy(&variable);

	return 1;
}

bool INSTR_popa()
{
	bool result = 1;
	bool processed = 0;
	variable_t *array = pool_reference(g_var_pool, g_operand2->integer);
	uchar type = array->type;

	for (int i = 0; result && i < g_operand1->integer; i++)
	{
		variable_t *index = var_stack_pop(g_vm_stack);

		switch (type)
		{
			case VAR_TYPE_STRING:
			{
				if (index->integer >= 1 && index->integer <= array->length)
				{
					variable_t *variable = var_stack_pop(g_vm_stack);

					processed = 1;
					type = VAR_TYPE_CHAR;
					array->string[index->integer - 1] = variable->character;

					var_destroy(&variable);
				}
				else
				{
					result = 0;
					printf("error : string index out of bounds.\n");
				}

				break;
			}

			case VAR_TYPE_ARRAY:
			{
				if (index->integer >= array->low && index->integer <= array->high)
				{
					array = array->data[index->integer - array->low];
					type = array->type;
				}
				else
				{
					result = 0;
					printf("error : array index out of bounds.\n");
				}

				break;
			}

			default:
			{
				result = 0;
				printf("error : cannot index regular variables.\n");
			}
		}

		var_destroy(&index);
	}

	if (!processed)
	{
		variable_t *variable = var_stack_pop(g_vm_stack);

		var_copy_value(array, variable);
		var_destroy(&variable);
	}

	return result;
}

bool INSTR_ldtrue()
{
	var_stack_push_boolean(g_vm_stack, 1);
	return 1;
}

bool INSTR_ldfalse()
{
	var_stack_push_boolean(g_vm_stack, 0);
	return 1;
}

bool INSTR_ldc()
{
	var_stack_push_char(g_vm_stack, g_operand1->character);
	return 1;
}

bool INSTR_ldf()
{
	var_stack_push_real(g_vm_stack, g_operand1->real);
	return 1;
}

bool INSTR_ldi()
{
	var_stack_push_integer(g_vm_stack, g_operand1->integer);
	return 1;
}

bool INSTR_lds()
{
	var_stack_push_string(g_vm_stack, g_operand1->string);
	return 1;
}

bool INSTR_chr()
{
	variable_t *variable = var_stack_top(g_vm_stack);

	if (variable->type != VAR_TYPE_INTEGER)
	{
		printf("error: invalid operand type.\n");
		return 0;
	}

	variable->type = VAR_TYPE_CHAR;
	variable->character = (char)variable->integer;

	return 1;
}

bool INSTR_real()
{
	variable_t *variable = var_stack_top(g_vm_stack);

	if (variable->type != VAR_TYPE_INTEGER)
	{
		printf("error: invalid operand type.\n");
		return 0;
	}

	variable->type = VAR_TYPE_REAL;
	variable->real = (double)variable->integer;

	return 1;
}

bool INSTR_str()
{
	variable_t *variable = var_stack_top(g_vm_stack);

	if (variable->type != VAR_TYPE_CHAR)
	{
		printf("error: invalid operand type.\n");
		return 0;
	}

	char buffer[2] = {0};

	buffer[0] = variable->character;

	variable->length = -1;
	variable->string = NULL;
	variable->type = VAR_TYPE_STRING;

	var_put_string(variable, buffer);

	return 1;
}

bool INSTR_int()
{
	variable_t *variable = var_stack_top(g_vm_stack);

	switch (variable->type)
	{
		case VAR_TYPE_REAL:		variable->integer = (int)variable->real; break;
		case VAR_TYPE_BOOLEAN:	variable->integer = (int)variable->boolean; break;
		case VAR_TYPE_CHAR:		variable->integer = (int)variable->character; break;

		default:
		{
			printf("error: invalid operand type.\n");
			return 0;
		}
	}

	variable->type = VAR_TYPE_INTEGER;

	return 1;
}

bool INSTR_bool()
{
	variable_t *variable = var_stack_top(g_vm_stack);

	if (variable->type != VAR_TYPE_INTEGER)
	{
		printf("error: invalid operand type.\n");
		return 0;
	}

	variable->type = VAR_TYPE_BOOLEAN;
	variable->boolean = variable->integer ? 1 : 0;

	return 1;
}

bool INSTR_lens()
{
	variable_t *string = var_stack_pop(g_vm_stack);

	var_stack_push_integer(g_vm_stack, strlen(string->string));
	var_destroy(&string);

	return 1;
}

bool INSTR_cats()
{
	variable_t *string1 = var_stack_pop(g_vm_stack);
	variable_t *string2 = var_stack_pop(g_vm_stack);

	int start = string1->length;

	string1->length += string2->length;
	string1->string = realloc(string1->string, string1->length + 1);

	strcpy(string1->string + start, string2->string);

	var_stack_push(g_vm_stack, string1);
	var_destroy(&string2);

	return 1;
}

bool INSTR_cpys()
{
	bool result = 1;
	variable_t *string = var_stack_pop(g_vm_stack);
	variable_t *start  = var_stack_pop(g_vm_stack);
	variable_t *length = var_stack_pop(g_vm_stack);

	if (start->integer <= 0 || length->integer <= 0)
	{
		result = 0;
		var_destroy(&string);
		printf("error: string index out of range.\n");
	}
	else
	{
		if (start->integer > string->length)
		{
			var_stack_push_string(g_vm_stack, NULL);
		}
		else
		{
			if (start->integer + length->integer - 1 > string->length)
				length->integer = string->length - start->integer + 1;

			if (start->integer > 1)
				memmove(string->string, string->string + start->integer - 1, length->integer);

			string->length = length->integer;
			string->string[length->integer] = 0;

			var_stack_push(g_vm_stack, string);
		}
	}

	var_destroy(&start);
	var_destroy(&length);

	return result;
}

bool INSTR_ints()
{
	bool result = 1;
	variable_t *source = var_stack_pop(g_vm_stack);
	variable_t *string = var_stack_pop(g_vm_stack);
	variable_t *start  = var_stack_pop(g_vm_stack);

	if (start->integer <= 0)
	{
		result = 0;
		var_destroy(&string);
		printf("error: string index out of range.\n");
	}
	else
	{
		string->string = realloc(string->string, string->length + source->length + 1);

		if (start->integer > string->length + 1)
		{
			memcpy(string->string + string->length, source->string, source->length);
		}
		else
		{
			memmove(
				string->string + start->integer + source->length - 1,
				string->string + start->integer - 1,
				string->length - start->integer + 1
			);

			memcpy(string->string + start->integer - 1, source->string, source->length);
		}

		string->string[string->length + source->length] = 0;

		var_stack_push(g_vm_stack, string);
	}

	var_destroy(&start);
	var_destroy(&source);

	return result;
}

bool INSTR_dels()
{
	bool result = 1;
	variable_t *string = var_stack_pop(g_vm_stack);
	variable_t *start  = var_stack_pop(g_vm_stack);
	variable_t *length = var_stack_pop(g_vm_stack);

	if (start->integer <= 0 || length->integer <= 0)
	{
		result = 0;
		var_destroy(&string);
		printf("error: string index out of range.\n");
	}
	else
	{
		if (start->integer <= string->length)
		{
			if (start->integer + length->integer - 1 >= string->length)
			{
				string->length = start->integer;
			}
			else
			{
				memmove(
					string->string + start->integer - 1,
					string->string + start->integer + length->integer - 1,
					string->length - start->integer - length->integer + 1
				);

				string->length -= length->integer;
			}

			string->string[string->length - 1] = 0;
		}

		var_stack_push(g_vm_stack, string);
	}

	var_destroy(&start);
	var_destroy(&length);

	return result;
}

bool INSTR_inv()
{
	variable_t *operand = var_stack_top(g_vm_stack);

	switch (operand->type)
	{
		case VAR_TYPE_REAL:		operand->real = -operand->real; break;
		case VAR_TYPE_INTEGER:	operand->integer = -operand->integer; break;
	}

	return 1;
}

bool INSTR_add()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_real(g_vm_stack, operand1->real + operand2->real); break;
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer + operand2->integer); break;
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_sub()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_real(g_vm_stack, operand1->real - operand2->real); break;
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer - operand2->integer); break;
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_mul()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_real(g_vm_stack, operand1->real * operand2->real); break;
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer * operand2->integer); break;
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_div()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	var_stack_push_real(g_vm_stack, operand1->real / operand2->real);
	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_idiv()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	var_stack_push_integer(g_vm_stack, operand1->integer / operand2->integer);
	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_mod()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	var_stack_push_integer(g_vm_stack, operand1->integer % operand2->integer);
	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_sin()
{
	variable_t *operand = var_stack_top(g_vm_stack);
	operand->real = sin(operand->real);
	return 1;
}

bool INSTR_cos()
{
	variable_t *operand = var_stack_top(g_vm_stack);
	operand->real = cos(operand->real);
	return 1;
}

bool INSTR_tan()
{
	variable_t *operand = var_stack_top(g_vm_stack);
	operand->real = tan(operand->real);
	return 1;
}

bool INSTR_sqrt()
{
	variable_t *operand = var_stack_top(g_vm_stack);
	operand->real = sqrt(operand->real);
	return 1;
}

bool INSTR_and()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer & operand2->integer); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean && operand2->boolean); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_or()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer | operand2->integer); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean || operand2->boolean); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_not()
{
	bool result = 1;
	variable_t *operand = var_stack_top(g_vm_stack);

	switch (operand->type)
	{
		case VAR_TYPE_BOOLEAN:	operand->boolean = !operand->boolean; break;
		case VAR_TYPE_INTEGER:	operand->integer = ~operand->integer; break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	return result;
}

bool INSTR_xor()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean ^ operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_integer(g_vm_stack, operand1->integer ^ operand2->integer); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_shl()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	var_stack_push_integer(g_vm_stack, operand1->integer << operand2->integer);
	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_shr()
{
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	var_stack_push_integer(g_vm_stack, operand1->integer >> operand2->integer);
	var_destroy(&operand1);
	var_destroy(&operand2);

	return 1;
}

bool INSTR_equ()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real == operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean == operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer == operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character == operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) == 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_neq()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real != operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean != operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer != operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character != operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) != 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_gt()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real > operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean > operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer > operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character > operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) > 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_lt()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real < operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean < operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer < operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character < operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) < 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_geq()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real >= operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean >= operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer >= operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character >= operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) >= 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_leq()
{
	bool result = 1;
	variable_t *operand2 = var_stack_pop(g_vm_stack);
	variable_t *operand1 = var_stack_pop(g_vm_stack);

	switch (operand1->type)
	{
		case VAR_TYPE_REAL:		var_stack_push_boolean(g_vm_stack, operand1->real <= operand2->real); break;
		case VAR_TYPE_BOOLEAN:	var_stack_push_boolean(g_vm_stack, operand1->boolean <= operand2->boolean); break;
		case VAR_TYPE_INTEGER:	var_stack_push_boolean(g_vm_stack, operand1->integer <= operand2->integer); break;
		case VAR_TYPE_CHAR:		var_stack_push_boolean(g_vm_stack, operand1->character <= operand2->character); break;
		case VAR_TYPE_STRING:	var_stack_push_boolean(g_vm_stack, strcmp(operand1->string, operand2->string) <= 0); break;

		default:
		{
			result = 0;
			printf("error: invalid operand type.\n");

			break;
		}
	}

	var_destroy(&operand1);
	var_destroy(&operand2);

	return result;
}

bool INSTR_br()
{
	g_bytecode->ip = g_operand1->integer;
	return 1;
}

bool INSTR_brt()
{
	variable_t *condition = var_stack_pop(g_vm_stack);

	if (condition->boolean)
		g_bytecode->ip = g_operand1->integer;

	var_destroy(&condition);

	return 1;
}

bool INSTR_brf()
{
	variable_t *condition = var_stack_pop(g_vm_stack);

	if (!condition->boolean)
		g_bytecode->ip = g_operand1->integer;

	var_destroy(&condition);

	return 1;
}

bool INSTR_call()
{
	callstack_invoke(g_bytecode->ip);
	g_bytecode->ip = g_operand1->integer;

	return 1;
}

bool INSTR_ret()
{
	g_bytecode->ip = callstack_return();
	return 1;
}

bool INSTR_dchar()
{
	g_previous_var = pool_def_char(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_dint()
{
	g_previous_var = pool_def_integer(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_dfloat()
{
	g_previous_var = pool_def_real(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_dstr()
{
	g_previous_var = pool_def_string(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_dbool()
{
	g_previous_var = pool_def_boolean(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_mkarr()
{
	var_make_array(g_previous_var, g_operand1->integer, g_operand2->integer);
	return 1;
}

bool INSTR_undef()
{
	pool_undef(g_var_pool, g_operand1->integer);
	return 1;
}

bool INSTR_read()
{
	for (int i = 0; i < g_operand1->integer; i++)
	{
		int count = 0;
		variable_t *data = var_stack_reference(g_vm_stack, i);

		switch (data->type)
		{
			case VAR_TYPE_REAL:			count = scanf("%lf", &data->real); break;
			case VAR_TYPE_INTEGER:		count = scanf("%d", &data->integer); break;
			case VAR_TYPE_CHAR:			count = scanf("%c", &data->character); break;
			case VAR_TYPE_STRING:
			{
				char buffer[1024];

				count = 1;
				fgets(buffer, 1024, stdin);
				var_put_string(data, buffer);
			}

			default:
			{
				printf("error: invalid operand type.");
				return 0;
			}
		}

		if (count == 0)
		{
			printf("error: invalid input format.");
			return 0;
		}
	}

	return 1;
}

bool INSTR_readln()
{
	int ptr = 0;
	char buffer[1024];

	fgets(buffer, 1024, stdin);

	for (int i = 0; i < g_operand1->integer; i++)
	{
		int count = -1;
		variable_t *data = var_stack_reference(g_vm_stack, i);

		while (count < 0)
		{
			switch (data->type)
			{
				case VAR_TYPE_REAL:			count = sscanf(buffer + ptr, "%lf%n", &data->real, &ptr); break;
				case VAR_TYPE_INTEGER:		count = sscanf(buffer + ptr, "%d%n", &data->integer, &ptr); break;
				case VAR_TYPE_CHAR:			count = sscanf(buffer + ptr, "%c%n", &data->character, &ptr); break;
				case VAR_TYPE_STRING:
				{
					var_put_string(data, buffer + ptr);

					ptr = 0;
					count = 1;
					fgets(buffer, 1024, stdin);

					break;
				}

				default:
				{
					printf("error: invalid operand type.");
					return 0;
				}
			}

			switch (count)
			{
				case 0:
				{
					printf("error: invalid input format.");
					return 0;
				}

				case -1:
				{
					ptr = 0;
					fgets(buffer, 1024, stdin);
				}
			}
		}
	}

	return 1;
}

bool INSTR_write()
{
	for (int i = 0; i < g_operand1->integer; i++)
	{
		variable_t *data = var_stack_pop(g_vm_stack);

		switch (data->type)
		{
			case VAR_TYPE_REAL:			printf("%f", data->real); break;
			case VAR_TYPE_STRING:		printf("%s", data->string); break;
			case VAR_TYPE_INTEGER:		printf("%d", data->integer); break;
			case VAR_TYPE_CHAR:			printf("%c", data->character); break;
			case VAR_TYPE_BOOLEAN:		printf("%s", data->boolean ? "TRUE" : "FALSE"); break;
		}

		var_destroy(&data);
	}

	return 1;
}

bool INSTR_writeln()
{
	for (int i = 0; i < g_operand1->integer; i++)
	{
		variable_t *data = var_stack_pop(g_vm_stack);

		switch (data->type)
		{
			case VAR_TYPE_REAL:			printf("%f", data->real); break;
			case VAR_TYPE_STRING:		printf("%s", data->string); break;
			case VAR_TYPE_INTEGER:		printf("%d", data->integer); break;
			case VAR_TYPE_CHAR:			printf("%c", data->character); break;
			case VAR_TYPE_BOOLEAN:		printf("%s", data->boolean ? "TRUE" : "FALSE"); break;
		}

		var_destroy(&data);
	}

	printf("\n");

	return 1;
}
