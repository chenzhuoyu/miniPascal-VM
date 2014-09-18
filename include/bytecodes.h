/*
 * bytecodes.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Bytecode Descriptors
 */

#ifndef __BYTECODES_H__
#define __BYTECODES_H__

#include "global.h"

#define OPERAND_NONE		0
#define OPERAND_CHAR		1
#define OPERAND_REAL		2
#define OPERAND_INTEGER		3
#define OPERAND_STRING		4

typedef struct _bytecode_desc_t
{
	char	*mnemonic;
	uchar	opcode;
	uchar	operand1;
	uchar	operand2;
} bytecode_desc_t;

static const bytecode_desc_t BYTECODES[] =
{
	{ "nop"		, 0xFF, OPERAND_NONE	, OPERAND_NONE		},
	{ "drop"	, 0x00, OPERAND_NONE	, OPERAND_NONE		},
	{ "dup"		, 0x01, OPERAND_NONE	, OPERAND_NONE		},
	{ "push"	, 0x02, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "pusha"	, 0x03, OPERAND_INTEGER	, OPERAND_INTEGER	},
	{ "pop"		, 0x04, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "popa"	, 0x05, OPERAND_INTEGER	, OPERAND_INTEGER	},
	{ "ldtrue"	, 0x06, OPERAND_NONE	, OPERAND_NONE		},
	{ "ldfalse"	, 0x07, OPERAND_NONE	, OPERAND_NONE		},
	{ "ldc"		, 0x08, OPERAND_CHAR	, OPERAND_NONE		},
	{ "ldf"		, 0x09, OPERAND_REAL	, OPERAND_NONE		},
	{ "ldi"		, 0x0A, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "lds"		, 0x0B, OPERAND_STRING	, OPERAND_NONE		},
	{ "chr"		, 0x0C, OPERAND_NONE	, OPERAND_NONE		},
	{ "real"	, 0x0D, OPERAND_NONE	, OPERAND_NONE		},
	{ "str"		, 0x0E, OPERAND_NONE	, OPERAND_NONE		},
	{ "int"		, 0x0F, OPERAND_NONE	, OPERAND_NONE		},
	{ "bool"	, 0x10, OPERAND_NONE	, OPERAND_NONE		},
	{ "lens"	, 0x11, OPERAND_NONE	, OPERAND_NONE		},
	{ "cats"	, 0x12, OPERAND_NONE	, OPERAND_NONE		},
	{ "cpys"	, 0x13, OPERAND_NONE	, OPERAND_NONE		},
	{ "ints"	, 0x14, OPERAND_NONE	, OPERAND_NONE		},
	{ "dels"	, 0x15, OPERAND_NONE	, OPERAND_NONE		},
	{ "inv"		, 0x16, OPERAND_NONE	, OPERAND_NONE		},
	{ "add"		, 0x17, OPERAND_NONE	, OPERAND_NONE		},
	{ "sub"		, 0x18, OPERAND_NONE	, OPERAND_NONE		},
	{ "mul"		, 0x19, OPERAND_NONE	, OPERAND_NONE		},
	{ "div"		, 0x1A, OPERAND_NONE	, OPERAND_NONE		},
	{ "idiv"	, 0x1B, OPERAND_NONE	, OPERAND_NONE		},
	{ "mod"		, 0x1C, OPERAND_NONE	, OPERAND_NONE		},
	{ "sin"		, 0x1D, OPERAND_NONE	, OPERAND_NONE		},
	{ "cos"		, 0x1E, OPERAND_NONE	, OPERAND_NONE		},
	{ "tan"		, 0x1F, OPERAND_NONE	, OPERAND_NONE		},
	{ "sqrt"	, 0x20, OPERAND_NONE	, OPERAND_NONE		},
	{ "and"		, 0x21, OPERAND_NONE	, OPERAND_NONE		},
	{ "or"		, 0x22, OPERAND_NONE	, OPERAND_NONE		},
	{ "not"		, 0x23, OPERAND_NONE	, OPERAND_NONE		},
	{ "xor"		, 0x24, OPERAND_NONE	, OPERAND_NONE		},
	{ "shl"		, 0x25, OPERAND_NONE	, OPERAND_NONE		},
	{ "shr"		, 0x26, OPERAND_NONE	, OPERAND_NONE		},
	{ "equ"		, 0x27, OPERAND_NONE	, OPERAND_NONE		},
	{ "neq"		, 0x28, OPERAND_NONE	, OPERAND_NONE		},
	{ "gt"		, 0x29, OPERAND_NONE	, OPERAND_NONE		},
	{ "lt"		, 0x2A, OPERAND_NONE	, OPERAND_NONE		},
	{ "geq"		, 0x2B, OPERAND_NONE	, OPERAND_NONE		},
	{ "leq"		, 0x2C, OPERAND_NONE	, OPERAND_NONE		},
	{ "br"		, 0x2D, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "brt"		, 0x2E, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "brf"		, 0x2F, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "call"	, 0x30, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "ret"		, 0x31, OPERAND_NONE	, OPERAND_NONE		},
	{ "dchar"	, 0x32, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "dint"	, 0x33, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "dfloat"	, 0x34, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "dstr"	, 0x35, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "dbool"	, 0x36, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "mkarr"	, 0x37, OPERAND_INTEGER	, OPERAND_INTEGER	},
	{ "undef"	, 0x38, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "read"	, 0x39, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "readln"	, 0x3A, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "write"	, 0x3B, OPERAND_INTEGER	, OPERAND_NONE		},
	{ "writeln"	, 0x3C, OPERAND_INTEGER	, OPERAND_NONE		},
	{ NULL }
};

#endif /* __BYTECODES_H__ */
