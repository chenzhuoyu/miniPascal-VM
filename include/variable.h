/*
 * variable.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Variables
 */

#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "global.h"

#include "list.h"

#define VAR_TYPE_CHAR		0
#define VAR_TYPE_REAL		1
#define VAR_TYPE_INTEGER	2
#define VAR_TYPE_STRING		3
#define VAR_TYPE_BOOLEAN	4
#define VAR_TYPE_ARRAY		5

typedef struct _variable_t
{
	node_t						node;
	uchar						type;

	union
	{
		char					character;
		double					real;
		int						integer;
		bool					boolean;

		struct
		{
			int					length;
			char				*string;
		};

		struct
		{
			int					low;
			int					high;
			struct _variable_t	**data;
		};
	};
} variable_t;

void var_destroy(variable_t **var);

variable_t *var_clone(variable_t *var);

variable_t *var_create_integer(int value);
variable_t *var_create_real(double value);
variable_t *var_create_char(char value);
variable_t *var_create_string(char *value);
variable_t *var_create_boolean(bool value);

void var_put_string(variable_t *dest, char *value);
void var_make_array(variable_t *var, int low, int high);

static inline void var_copy_value(variable_t *dest, variable_t *src)
{
	switch (src->type)
	{
		case VAR_TYPE_REAL:			dest->real = src->real;				break;
		case VAR_TYPE_BOOLEAN:		dest->boolean = src->boolean;		break;
		case VAR_TYPE_INTEGER:		dest->integer = src->integer;		break;
		case VAR_TYPE_CHAR:			dest->character = src->character;	break;
		case VAR_TYPE_STRING:		var_put_string(dest, src->string);	break;
		case VAR_TYPE_ARRAY:
		{
			dest->low = src->low;
			dest->high = src->high;
			dest->data = malloc(sizeof(variable_t *) * (src->high - src->low + 1));
			for (int i = 0; i <= src->high - src->low; i++) dest->data[i] = var_clone(src->data[i]);

			break;
		}
	}
}

#endif /* __VARIABLE_H__ */
