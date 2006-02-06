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
#define PUSH_TEMP tempstack++; BF_PUSH
#define BF_POP printf("<<<-<<"); fflush(stdout)
#define POP_TEMP tempstack--; BF_POP
#define POP_TEMPS for (; tempstack > 0; tempstack--) BF_POP

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

/* tempstack is the number of stack cells used for temporary variables - this
 * must be added to the number gotten from the variable list to properly access
 * a variable */
extern int tempstack;

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
 * effect: a variable is pushed onto the variable stack, and the previous
 *         variable's depth is set
 */
void pushVar(const char *name, int width);

/* popVar
 * input: none
 * output: none
 * effect: a variable is popped from the variable stack
 */
void popVar();

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
