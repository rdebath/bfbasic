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

#include <string.h>

#include "generator.h"

struct block *curblock = NULL;
int tempstack = 0;
struct var *curvar = NULL;

/* pushNamedBlock
 * input: name of the block to push
 * output: none
 * effect: curblock now points at a block with the name given
 */
void pushNamedBlock(const char *name)
{
    struct block *prevblock = curblock;
    NEW(curblock, struct block);
    
    curblock->next = prevblock;
    curblock->name = strdup(name);
    if (!curblock->name) { perror("strdup"); exit(1); }
    curblock->num = 0;
    curblock->vars = 0;
    curblock->stack = 0;
}

/* pushBlock
 * input: none
 * output: none
 * effect: curblock now points at a block with the previous name, but a new
 *         number
 */
void pushBlock()
{
    struct block *prevblock = curblock;
    NEW(curblock, struct block);
    
    if (!prevblock) {
        fprintf(stderr, "Internal compiler error in pushBlock()\n");
        exit(1);
    }
    
    curblock->next = prevblock;
    curblock->name = strdup(prevblock->name);
    if (!curblock->name) { perror("strdup"); exit(1); }
    curblock->num = prevblock->num + 1;
    curblock->vars = 0;
    curblock->stack = 0;
}

/* pushCall
 * input: the function to call
 * output: none
 * effect: the function is called with the return address of pushBlock()
 */
void pushCall(const char *func)
{
    if (!curblock) {
        fprintf(stderr, "Internal compiler error in pushCall()\n");
        exit(1);
    }
    
    printf("(*%s!%d)(%s)", curblock->name, curblock->num + 1, func);
    
    pushBlock();
}

/* popNamedBlock
 * input: none
 * output: none
 * effect: pop down to the last named block
 */
void popNamedBlock()
{
    if (!curblock) {
        fprintf(stderr, "Internal compiler error 1 in popNamedBlock()\n");
        exit(1);
    }
    
    /* keep popping until we pop one with num=0 */
    while (curblock->num > 0) {
        popBlock();
        
        if (!curblock) {
            fprintf(stderr, "Internal compiler error 2 in popNamedBlock()\n");
            exit(1);
        }
    }
    
    /* this one has num = 0, pop it */
    popBlock();
}

/* popBlock
 * input: none
 * output: none
 * effect: pop a single block
 */
void popBlock()
{
    struct block *nextblock;
    int i;
    
    if (!curblock) {
        fprintf(stderr, "Internal compiler error in popBlock()\n");
        exit(1);
    }
    
    /* pop off the vars */
    for (i = 0; i < curblock->vars; i++) {
        popVar();
    }
    
    free(curblock->name);
    
    nextblock = curblock->next;
    free(curblock);
    curblock = nextblock;
}

/* outBlock
 * input: none
 * output: none
 * effect: "\n", the appropriate block name and ": " are printed on stdout
 */
void outBlock()
{
    if (!curblock) {
        fprintf(stderr, "Internal compiler error in outBlock()\n");
        exit(1);
    }

    /* if the number is nonzero, name!num, otherwise just name */
    if (curblock->num) {
        printf("\n%s!%d: ", curblock->name, curblock->num);
    } else {
        printf("\n%s: ", curblock->name);
    }
    
    fflush(stdout);
}

/* pushVar
 * input: a variable name and width in stack cells
 * output: none
 * effect: a variable is pushed onto the variable stack, and the previous
 *         variable's depth is set
 */
void pushVar(const char *name, int width)
{
    struct var *prevvar;
    
    if (!curblock) {
        fprintf(stderr, "Internal compiler error in pushVar()\n");
        exit(1);
    }
    
    /* add the var to the block */
    curblock->vars++;
    curblock->stack += width;
    
    /* and push the var on the stack */
    prevvar = curvar;
    NEW(curvar, struct var);
    
    curvar->next = prevvar;
    curvar->name = strdup(name);
    if (!curvar->name) { perror("strdup"); exit(1); }
    curvar->width = width;
}

/* popVar
 * input: none
 * output: none
 * effect: a variable is popped from the variable stack
 */
void popVar()
{
    struct var *nextvar;
    
    if (!curvar) {
        fprintf(stderr, "Internal compiler error in popVar()\n");
        exit(1);
    }
    
    BF_POP;
    
    free(curvar->name);
    nextvar = curvar->next;
    free(curvar);
    curvar = nextvar;
}

/* varDepth
 * input: variable name
 * output: the depth of the variable in the current stack or -1 on error
 * effect: none
 */
int varDepth(const char *name)
{
    int depth = 0;
    struct var *cur;
    
    /* start with the top */
    cur = curvar;
    
    while (cur) {
        /* if it matches, return the current depth */
        if (!strcmp(cur->name, name))
            return depth;
        
        depth += cur->width;
        cur = cur->next;
    }
    
    /* no match! */
    return -1;
}

/* blockDepth
 * input: none
 * output: the depth of the current block
 * effect: none
 */
int blockDepth()
{
    int depth = 0;
    struct var *cur;
    
    /* start with the top */
    cur = curvar;
    
    while (cur) {
        /* add the current depth */
        depth += cur->width;
        cur = cur->next;
    }
    
    return depth;
}
