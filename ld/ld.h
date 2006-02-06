#ifndef LD_H
#define LD_H

struct block {
    struct block *next, *prev;;
    int num;
    char *label;
    char *code;
};

extern struct block *curblock;

/* blockNum
 * input: the name of a symbol
 * output: its number (or -1 on error)
 * effect: none
 */
int blockNum(const char *label);

#endif
