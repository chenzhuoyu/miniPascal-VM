/*
 * io.c
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Bytecode IO
 */

#include "io.h"
#include "bytecodes.h"

static const bytecode_desc_t *get_descriptor(uchar opcode)
{
	const bytecode_desc_t *desc = BYTECODES;
	while (desc->mnemonic && (desc->opcode != opcode)) desc++;
	return desc->mnemonic ? desc : NULL;
}

static void adjust_bytecode_ip(bytecode_t *bytecode, uchar operand)
{
	switch (operand)
	{
		case OPERAND_CHAR:		bytecode->ip += sizeof(char);									break;
		case OPERAND_REAL:		bytecode->ip += sizeof(double);									break;
		case OPERAND_INTEGER:	bytecode->ip += sizeof(int);									break;
		case OPERAND_STRING:	bytecode->ip += strlen(bytecode->buffer + bytecode->ip) + 1;	break;
	}
}

void io_close_bytecode(bytecode_t *bytecode)
{
	bytecode->ip = -1;
	bytecode->size = -1;

	free(bytecode->buffer);
}

bool io_open_bytecode(bytecode_t *bytecode, const char *name)
{
	FILE *file = fopen(name, "rb");

	if (file == NULL) return 0;

	fseek(file, 0, SEEK_END);

	bytecode->ip = 0;
	bytecode->size = ftell(file);
	bytecode->buffer = malloc(bytecode->size);

	rewind(file);
	fread(bytecode->buffer, 1, bytecode->size, file);
	fclose(file);

	return 1;
}

bool io_fetch_opcode(bytecode_t *bytecode, opcode_t *opcode)
{
	if (bytecode->ip >= bytecode->size) return 0;

	const bytecode_desc_t *desc = get_descriptor(bytecode->buffer[bytecode->ip]);

	if (desc == NULL) return 0;

	opcode->ip = bytecode->ip++;
	opcode->opcode = desc->opcode;
	opcode->mnemonic = desc->mnemonic;

	switch (desc->operand1)
	{
		default:				opcode->operand1 = NULL; break;
		case OPERAND_CHAR:		opcode->operand1 = var_create_char(bytecode->buffer[bytecode->ip]); break;
		case OPERAND_STRING:	opcode->operand1 = var_create_string(bytecode->buffer + bytecode->ip); break;
		case OPERAND_REAL:		opcode->operand1 = var_create_real(*(double *)&bytecode->buffer[bytecode->ip]); break;
		case OPERAND_INTEGER:	opcode->operand1 = var_create_integer(*(int *)&bytecode->buffer[bytecode->ip]); break;
	}

	adjust_bytecode_ip(bytecode, desc->operand1);

	switch (desc->operand2)
	{
		default:				opcode->operand2 = NULL; break;
		case OPERAND_CHAR:		opcode->operand2 = var_create_char(bytecode->buffer[bytecode->ip]); break;
		case OPERAND_STRING:	opcode->operand2 = var_create_string(bytecode->buffer + bytecode->ip); break;
		case OPERAND_REAL:		opcode->operand2 = var_create_real(*(double *)&bytecode->buffer[bytecode->ip]); break;
		case OPERAND_INTEGER:	opcode->operand2 = var_create_integer(*(int *)&bytecode->buffer[bytecode->ip]); break;
	}

	adjust_bytecode_ip(bytecode, desc->operand2);

	return 1;
}
