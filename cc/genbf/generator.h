/*
 * Copyright (c) 2006  Gregor Richards
 * 
 * This file is part of C2BF.
 * 
 * C2BF is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * C2BF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with C2BF; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <signal.h>

#define UNIMPL(x) \
{ \
    fprintf(stderr, "Unimplemented generation: %s\n", x); \
    exit(1); \
}

#define ERROR(x, y) \
{ \
    fprintf(stderr, "ERROR in generation %s: %s\n", x, y); \
    exit(1); \
}

#define NEW(x, y) \
{ \
    x = (y *) malloc(sizeof(y)); \
    if (!x) { \
        perror("malloc"); \
        exit(1); \
    } \
}

#define BF_PUSH printf(">>+>>>"); fflush(stdout)
#define BF_POP printf("<<<-<<"); fflush(stdout)

/* struct block holds information on what block of code we're in, how many
 * variables it uses, and how deep the stack is */
struct block {
    struct block *next;
    char *name;
    int num; /* the name in the output file is name!num (except where num is
              * 0) */
    int vars;
    int stack;
};
extern struct block *curblock;

/* pushNamedBlock
 * input: name of the block to push
 * output: none
 * effect: curblock now points at a block with the name given
 */
void pushNamedBlock(const char *name);

/* pushSubBlock
 * input: an offset
 * output: none
 * effect: curblock now points at a block with the previous name!num+1+offset,
 *         and num 0
 */
void pushSubBlock(int offset);

/* pushBlock
 * input: none
 * output: none
 * effect: curblock now points at a block with the previous name, but a new
 *         number
 */
void pushBlock();

/* pushCall
 * input: the function to call
 * output: none
 * effect: the function is called with the return address of pushBlock()
 */
void pushCall(const char *func);

/* popNamedBlock
 * input: none
 * output: none
 * effect: pop down to the last named block
 */
void popNamedBlock();

/* popBlock
 * input: none
 * output: none
 * effect: pop a single block
 */
void popBlock();

/* outBlock
 * input: none
 * output: none
 * effect: "\n", the appropriate block name and ": " are printed on stdout
 */
void outBlock();

/* struct var is a linear linked list of current variables */
struct var {
    struct var *next;
    char *name;
    int width;
};
extern struct var *curvar;

/* pushVar
 * input: a variable name and width in stack cells
 * output: none
 * effect: a variable is pushed onto the internal variable stack
 */
void pushVar(const char *name, int width);

/* pushTempVar
 * input: a variable width in stack cells
 * output: none
 * effect: an unnamed variable is pushed onto the internal variable stack
 */
void pushTempVar(int width);

/* popVar
 * input: none
 * output: none
 * effect: a variable is popped from the variable stack (both internal and BF)
 */
void popVar();

/* ignoreVar
 * input: none
 * output: none
 * effect: a variable is popped from the internal stack but left in the BF stack
 */
void ignoreVar();

/* varDepth
 * input: variable name
 * output: the depth of the variable in the current stack or -1 on error
 * effect: none
 */
int varDepth(const char *name);

/* blockDepth
 * input: none
 * output: the depth of the current block
 * effect: none
 */
int blockDepth();

#endif
