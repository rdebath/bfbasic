/*====================================================================
' BFI -- BF interpreter
' Filename : bfi.c
' Language : ANSI C 1989
' Version  : 0.10
' Copyright: (C) 2004 Jeffry Johnston
'
' This program is free software; you can redistribute it and/or modify
' it under the terms of the GNU General Public License as published by
' the Free Software Foundation. See the file LICENSE for more details.
'
' Taken from Brian Raiter's BF page, on Muppetlabs:
' (http://www.muppetlabs.com/~breadbox/bf)
'
' A [Brainf***] program has an implicit byte pointer, called "the
' pointer", which is free to move around within an array of 30000
' bytes, initially all set to zero.  The pointer itself is initialized
' to point to the beginning of this array.
'
' The [Brainf***] programming language consists of eight commands,
' each of which is represented as a single character.
'
' >     Increment the pointer.
' <     Decrement the pointer.
' +     Increment the byte at the pointer.
' -     Decrement the byte at the pointer.
' .     Output the byte at the pointer.
' ,     Input a byte and store it in the byte at the pointer.
' [     Jump forward past the matching ] if the byte at the pointer
        is zero.
' ]     Jump backward to the matching [ unless the byte at the pointer
'       is zero.
'
' Version history:
' 0.10    24 Jun 2004 Initial release
====================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match(int);
char * src;
long lof, ip = -1;

/* main */
int main(int argc, char * argv[]) {
  FILE * infile;
  long mp = 0, maxmp = 1023;
  int n;
  unsigned short int * mem;

  /* parse command line arguments */
  if (argc != 2 || argv[1][0] == '-' || argv[1][0] == '/') {
    puts("Usage: bfi file.b");
    return EXIT_FAILURE;
  }
  /* open file */
  infile = fopen(argv[1], "rb");
  if (infile == NULL) {
    puts("Error opening input file");
    return EXIT_FAILURE;
  }
  /* determine length of file */
  if (fseek(infile, 0L, SEEK_END) != 0) {
    puts("Error determining length of input file");
    return EXIT_FAILURE;
  }
  lof = ftell(infile);
  if (lof == -1) {
    puts("Error determining length of input file");
    return EXIT_FAILURE;
  }
  if (fseek(infile, 0L, SEEK_SET) != 0) {
    puts("Error determining length of input file");
    return EXIT_FAILURE;
  }
  if (lof == 0) { /* empty file, nothing to do */
    return EXIT_SUCCESS;
  }
  /* read file */
  src = (char *) calloc(lof + 2, sizeof(char));
  if (src == NULL) {
    puts("Program too big to fit in memory");
    return EXIT_FAILURE;
  }
  if (fread(src, sizeof(char), lof, infile) < (unsigned) lof) {
    puts("Error reading input file");
    return EXIT_FAILURE;
  }
  /* close file */
  if (fclose(infile) == -1) {
    puts("Error closing input file");
    return EXIT_FAILURE;
  }
  /* allocate initial memory block */
  mem = calloc(1024, sizeof(unsigned short int));
  if (mem == NULL) {
    puts("Out of memory");
    return EXIT_FAILURE;
  }
  /* interpret program */
  while (++ip < lof) {
    switch (src[ip]) {
    /*
    case '#':
      printf("mp:%li r1:%i r2:%i r3:%i r4:%i\n", mp, mem[5], mem[6], mem[7], mem[8]);
      break;
    */
    case '>':
      if (mp >= maxmp) {
        /* putchar('$'); */
        mem = realloc(mem, (maxmp + 1024) * sizeof(unsigned short int));
        if (mem == NULL) {
          puts("Out of memory");
          return EXIT_FAILURE;
        }
        for (n = 1; n <= 1024; n++) {
          mem[maxmp + n] = 0;
        }
        maxmp += 1024;
      }
      mp++;
      break;
    case '<':
      if (mp <= 0) {
        puts("Memory pointer error");
        return EXIT_FAILURE;
      }
      mp--;
      break;
    case '+':
      mem[mp]++;
      break;
    case '-':
      mem[mp]--;
      break;
    case '.':
      putchar(mem[mp]);
      break;
    case ',':
      n = getchar();
      if (n < 0) {
        n = 0;
      }
      mem[mp] = n;
      break;
    case '[':
      if (src[ip + 1] == '-' && src[ip + 2] == ']') {
        mem[mp] = 0;
        ip += 2;
      } else if (mem[mp] == 0) {
        match(1);
      }
      break;
    case ']':
      match(-1);
      ip--;
      break;
    }
  }
  return EXIT_SUCCESS;
}

/* match */
void match(int dir) {
  int level = 1;
  char ipvalue;
  do {
    ip += dir;
    if (ip < 0 || ip >= lof) {
      puts("Mismatched brackets");
      exit(EXIT_FAILURE);
    }
    ipvalue = src[ip];
    if (ipvalue == 91) {
      level += dir;
    } else if (ipvalue == 93) {
      level -= dir;
    }
  } while (level > 0);
}
