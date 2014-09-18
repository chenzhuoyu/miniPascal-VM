/*
 * io.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Bytecode IO
 */

#ifndef __IO_H__
#define __IO_H__

#include <sys/stat.h>

#include "global.h"
#include "variable.h"

typedef struct _bytecode_t
{
	int		ip;
	int		size;
	char	*buffer;
} bytecode_t;

typedef struct _opcode_t
{
	uint		ip;
	uchar		opcode;
	variable_t	*operand1;
	variable_t	*operand2;
	char		*mnemonic;
} opcode_t;

void io_close_bytecode(bytecode_t *bytecode);
bool io_open_bytecode(bytecode_t *bytecode, const char *name);

bool io_fetch_opcode(bytecode_t *bytecode, opcode_t *opcode);

#endif /* __IO_H__ */
