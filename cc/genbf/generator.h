#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>

#define UNIMPL(x) \
{ \
    fprintf(stderr, "Unimplemented generation: %s\n", x); \
}

#define NEW(x, y) \
{ \
    x = (y *) malloc(sizeof(y)); \
    if (!x) { \
        perror("malloc"); \
        exit(1); \
    } \
}

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
    int depth; /* relative to the last var */
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

#endif
