/*
 * variable.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal Bytecode VM
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

#include "io.h"
#include "instr.h"
#include "mempool.h"
#include "varpool.h"
#include "varstack.h"
#include "variable.h"
#include "callstack.h"

typedef bool (* instr_fn_t)();

static const instr_fn_t INSTR_FN[] =
{
	INSTR_drop,
	INSTR_dup,
	INSTR_push,
	INSTR_pusha,
	INSTR_pop,
	INSTR_popa,
	INSTR_ldtrue,
	INSTR_ldfalse,
	INSTR_ldc,
	INSTR_ldf,
	INSTR_ldi,
	INSTR_lds,
	INSTR_chr,
	INSTR_real,
	INSTR_str,
	INSTR_int,
	INSTR_bool,
	INSTR_lens,
	INSTR_cats,
	INSTR_cpys,
	INSTR_ints,
	INSTR_dels,
	INSTR_inv,
	INSTR_add,
	INSTR_sub,
	INSTR_mul,
	INSTR_div,
	INSTR_idiv,
	INSTR_mod,
	INSTR_sin,
	INSTR_cos,
	INSTR_tan,
	INSTR_sqrt,
	INSTR_and,
	INSTR_or,
	INSTR_not,
	INSTR_xor,
	INSTR_shl,
	INSTR_shr,
	INSTR_equ,
	INSTR_neq,
	INSTR_gt,
	INSTR_lt,
	INSTR_geq,
	INSTR_leq,
	INSTR_br,
	INSTR_brt,
	INSTR_brf,
	INSTR_call,
	INSTR_ret,
	INSTR_dchar,
	INSTR_dint,
	INSTR_dfloat,
	INSTR_dstr,
	INSTR_dbool,
	INSTR_mkarr,
	INSTR_undef,
	INSTR_read,
	INSTR_readln,
	INSTR_write,
	INSTR_writeln,
};

bool run(opcode_t *opcode)
{
	instr_set_operands(opcode->operand1, opcode->operand2);
	return INSTR_FN[opcode->opcode]();
}

void disassemble(bytecode_t *bytecode, opcode_t *opcode)
{
	char buffer[256];
	memset(buffer, ' ', sizeof(buffer));
	int ptr = sprintf(buffer, "%.8X %-8s ", opcode->ip, opcode->mnemonic);

	if (opcode->operand1)
	{
		switch (opcode->operand1->type)
		{
			case VAR_TYPE_CHAR:		ptr += sprintf(buffer + ptr, "'%c'", opcode->operand1->character);	break;
			case VAR_TYPE_REAL:		ptr += sprintf(buffer + ptr, "%.2f", opcode->operand1->real);		break;
			case VAR_TYPE_STRING:	ptr += sprintf(buffer + ptr, "'%s'", opcode->operand1->string);		break;
			case VAR_TYPE_INTEGER:	ptr += sprintf(buffer + ptr, "$%.8X", opcode->operand1->integer);	break;
		}
	}

	if (opcode->operand2)
	{
		switch (opcode->operand2->type)
		{
			case VAR_TYPE_CHAR:		ptr += sprintf(buffer + ptr, ", '%c'", opcode->operand2->character);	break;
			case VAR_TYPE_REAL:		ptr += sprintf(buffer + ptr, ", %.2f", opcode->operand2->real);			break;
			case VAR_TYPE_STRING:	ptr += sprintf(buffer + ptr, ", '%s'", opcode->operand2->string);		break;
			case VAR_TYPE_INTEGER:	ptr += sprintf(buffer + ptr, ", $%.8X", opcode->operand2->integer);		break;
		}
	}

	buffer[47] = ' ';
	buffer[48] = '|';
	buffer[49] = ' ';
	buffer[ptr] = ' ';

	for (int p = 50, i = opcode->ip; i < bytecode->ip; i++)
		p += sprintf(buffer + p, "%.2X ", (uchar)bytecode->buffer[i]);

	printf("%s\n", buffer);
}

void usage()
{
	printf("usage: mpvm -[h|d|u] filename\n");
	printf("    -h        display this message\n");
	printf("    -d        debug\n");
	printf("    -u        disassemble\n");
	printf("\n");
	exit(1);
}

void rtrim(char *s)
{
	while (*s) (*s = (*s == '\r' || *s == '\n') ? 0 : *s), s++;
}

void dump_variable(variable_t *var)
{
	switch (var->type)
	{
		case VAR_TYPE_REAL:		printf("-> real     %f\n", var->real); break;
		case VAR_TYPE_INTEGER:	printf("-> integer  %i\n", var->integer); break;
		case VAR_TYPE_STRING:	printf("-> string   '%s'\n", var->string); break;
		case VAR_TYPE_CHAR:		printf("-> char     %c\n", var->character); break;
		case VAR_TYPE_BOOLEAN:	printf("-> boolean  %s\n", var->boolean ? "TRUE" : "FALSE"); break;
		case VAR_TYPE_ARRAY:
		{
			printf("-> array    {\n");
			for (int i = 0; i <= var->high - var->low; i++)
			{
				switch (var->data[i]->type)
				{
					case VAR_TYPE_REAL:		printf("    [%d] real     %f\n", i, var->data[i]->real); break;
					case VAR_TYPE_INTEGER:	printf("    [%d] integer  %i\n", i, var->data[i]->integer); break;
					case VAR_TYPE_STRING:	printf("    [%d] string   '%s'\n", i, var->data[i]->string); break;
					case VAR_TYPE_CHAR:		printf("    [%d] char     %c\n", i, var->data[i]->character); break;
					case VAR_TYPE_BOOLEAN:	printf("    [%d] boolean  %s\n", i, var->data[i]->boolean ? "TRUE" : "FALSE"); break;
				}

				if ((i + 1) % 20 == 0)
				{
					char choice[1025];
					printf("continue ? (Y/n) ");
					fgets(choice, 1024, stdin);
					if (choice[0] == 'n') break;
				}
			}

			printf("   }\n");
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("error: no input file.\n");
		return 1;
	}

	bool debug = 0;
	bool disasm = 0;
	char *file = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)			usage();
		else if (strcmp(argv[i], "-d") == 0)	debug = 1;
		else if (strcmp(argv[i], "-u") == 0)	disasm = 1;
		else if (file == NULL)					file = argv[i];
	}

	if (!file || (debug && disasm)) usage();

	list_t vmpool;
	var_stack_t vmstack;

	opcode_t opcode;
	bytecode_t bytecode;

	if (!io_open_bytecode(&bytecode, file))
	{
		printf("error: cannot read bytecode file \"%s\".\n", file);
		return 2;
	}

	mem_init();
	callstack_init();
	instr_init(&bytecode, &vmpool, &vmstack);

	if (disasm)
	{
		while (io_fetch_opcode(&bytecode, &opcode))
		{
			disassemble(&bytecode, &opcode);

			if (opcode.operand1) var_destroy(&opcode.operand1);
			if (opcode.operand2) var_destroy(&opcode.operand2);
		}
	}
	else if (!debug)
	{
		pool_init(&vmpool);
		var_stack_init(&vmstack);

		while (bytecode.ip >= 0 && io_fetch_opcode(&bytecode, &opcode))
		{
			if (opcode.opcode != 0xFF)
			{
				bool result = run(&opcode);

				if (opcode.operand1) var_destroy(&opcode.operand1);
				if (opcode.operand2) var_destroy(&opcode.operand2);

				if (!result)
				{
					printf("    ip = 0x%.8x\n", opcode.ip);
					break;
				}
			}
		}

		pool_destroy(&vmpool);
		var_stack_destroy(&vmstack);
	}
	else
	{
		char command[1025] = {0};

		bool running = 0;
		bool debugging = 1;
		int breakpoints[256];

		pool_init(&vmpool);
		var_stack_init(&vmstack);

		printf("miniPascal VM Built-in Bytecode Debugger\n\n");

		for (int i = 0; i < 256; i++) breakpoints[i] = -1;

		while (bytecode.ip >= 0 && io_fetch_opcode(&bytecode, &opcode))
		{
			for (int i = 0; i < 256; i++)
			{
				if (breakpoints[i] == opcode.ip)
				{
					running = 0;
					printf("# breakpoint (%d) triggered.\n", i);
				}
			}

			if (!running)
			{
				disassemble(&bytecode, &opcode);

				while (1)
				{
					do
					{
						char buffer[1025] = {0};

						printf("> ");
						fgets(buffer, 1024, stdin);
						rtrim(buffer);

						if (strlen(buffer) > 0) strcpy(command, buffer);
					} while (strlen(command) == 0);

					char cmd;

					if (sscanf(command, "%c", &cmd) <= 0) continue;

					switch (cmd)
					{
						default: continue;

						case 't': break;
						case 'g': running = 1; break;
						case 'q': debugging = 0; break;

						case 'h':
						{
							printf("h                       display this message\n");
							printf("x <id>                  dump variable <id>\n");
							printf("s <index> [count=1]     dump stack[<index>], 0 represents stack top\n");
							printf("u <addr> [count=1]      disassemble\n");
							printf("b <addr>                set breakpoint\n");
							printf("d <index>               delete breakpoint\n");
							printf("t                       single step\n");
							printf("g                       run\n");
							printf("q                       quit\n");
							continue;
						}

						case 'd':
						{
							int index;

							if (sscanf(command, "%*c%d", &index) <= 0)
							{
								printf("error: breakpoint number expected.\n");
								continue;
							}

							if (index < 0 || index >= 256)
							{
								printf("error: breakpoint number out of bound.\n");
								continue;
							}

							breakpoints[index] = -1;
							continue;
						}

						case 'x':
						{
							int id;

							if (sscanf(command, "%*c%x", &id) <= 0)
							{
								printf("error: variable id expected.\n");
								continue;
							}

							variable_t *variable = pool_reference(&vmpool, id);

							if (variable == NULL)
							{
								printf("error: variable %d is undefined.\n", id);
								continue;
							}

							dump_variable(variable);
							continue;
						}

						case 's':
						{
							int index;
							int count = 1;

							if (sscanf(command, "%*c%d", &index) <= 0)
							{
								printf("error: stack index expected.\n");
								continue;
							}

							int buffer;
							if (sscanf(command, "%*c%*d%d", &buffer) > 0) count = buffer;

							while (count--)
							{
								variable_t *stack = var_stack_reference(&vmstack, index++);

								if (stack == NULL)
								{
									printf("error: stack index out of bound.\n");
									break;
								}

								dump_variable(stack);
							}

							continue;
						}

						case 'u':
						{
							int count = 1;
							int address = 0;
							int ip = bytecode.ip;

							opcode_t instr;

							if (sscanf(command, "%*c%x", &address) <= 0)
							{
								printf("error: disassemble address expected.\n");
								continue;
							}

							int buffer;
							if (sscanf(command, "%*c%*x%d", &buffer) > 0) count = buffer;

							bytecode.ip = address;

							while (count--)
							{
								if (!io_fetch_opcode(&bytecode, &instr))
								{
									printf("error: meet EOF.\n");
									break;
								}

								disassemble(&bytecode, &instr);
							}

							bytecode.ip = ip;

							continue;
						}

						case 'b':
						{
							int breakpoint = 0;

							if (sscanf(command + 1, "%*c%x", &breakpoint) <= 0)
							{
								printf("error: breakpoint address expected.\n");
								continue;
							}

							for (int i = 0; i < 256; i++)
							{
								if (breakpoints[i] < 0)
								{
									printf("# new breakpoint (%d) at 0x%.8x\n", i, breakpoint);
									breakpoints[i] = breakpoint;
									breakpoint = -1;

									break;
								}
							}

							if (breakpoint >= 0) printf("error: too many breakpoints.\n");

							continue;
						}
					}

					break;
				}
			}

			if (!debugging) break;

			if (opcode.opcode != 0xFF)
			{
				bool result = run(&opcode);

				if (opcode.operand1) var_destroy(&opcode.operand1);
				if (opcode.operand2) var_destroy(&opcode.operand2);

				if (!result)
				{
					running = 0;
					printf("    ip = 0x%.8x\n", opcode.ip);
				}
			}
		}

		printf("\nprogram exited normally.\n\n");

		pool_destroy(&vmpool);
		var_stack_destroy(&vmstack);
	}

	mem_destroy();
	callstack_destroy();
	io_close_bytecode(&bytecode);

	return 0;
}
