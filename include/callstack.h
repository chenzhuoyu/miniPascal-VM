/*
 * callstack.h
 *
 *  Created on: 2013-2-7
 *      Author: desperedo
 *
 *	miniPascal VM Callstack
 */

#ifndef __CALLSTACK_H__
#define __CALLSTACK_H__

#include "global.h"

#define CALLSTACK_BUCKET_SIZE		512

void callstack_init();
void callstack_destroy();

int callstack_return();
void callstack_invoke(int ip);

#endif /* __CALLSTACK_H__ */
