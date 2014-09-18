/*
 * instr.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Instructions
 */

#ifndef __INSTR_H__
#define __INSTR_H__

#include "io.h"
#include "varpool.h"
#include "varstack.h"
#include "variable.h"
#include "callstack.h"

void instr_set_operands(variable_t *operand1, variable_t *operand2);
void instr_init(bytecode_t *bytecode, list_t *pool, var_stack_t *vmstack);

bool INSTR_drop();
bool INSTR_dup();
bool INSTR_push();
bool INSTR_pusha();
bool INSTR_pop();
bool INSTR_popa();
bool INSTR_ldtrue();
bool INSTR_ldfalse();
bool INSTR_ldc();
bool INSTR_ldf();
bool INSTR_ldi();
bool INSTR_lds();
bool INSTR_chr();
bool INSTR_real();
bool INSTR_str();
bool INSTR_int();
bool INSTR_bool();
bool INSTR_lens();
bool INSTR_cats();
bool INSTR_cpys();
bool INSTR_ints();
bool INSTR_dels();
bool INSTR_inv();
bool INSTR_add();
bool INSTR_sub();
bool INSTR_mul();
bool INSTR_div();
bool INSTR_idiv();
bool INSTR_mod();
bool INSTR_sin();
bool INSTR_cos();
bool INSTR_tan();
bool INSTR_sqrt();
bool INSTR_and();
bool INSTR_or();
bool INSTR_not();
bool INSTR_xor();
bool INSTR_shl();
bool INSTR_shr();
bool INSTR_equ();
bool INSTR_neq();
bool INSTR_gt();
bool INSTR_lt();
bool INSTR_geq();
bool INSTR_leq();
bool INSTR_br();
bool INSTR_brt();
bool INSTR_brf();
bool INSTR_call();
bool INSTR_ret();
bool INSTR_dchar();
bool INSTR_dint();
bool INSTR_dfloat();
bool INSTR_dstr();
bool INSTR_dbool();
bool INSTR_mkarr();
bool INSTR_undef();
bool INSTR_read();
bool INSTR_readln();
bool INSTR_write();
bool INSTR_writeln();

#endif /* __INSTR_H__ */
