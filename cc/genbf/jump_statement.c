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

#include "../genbf.h"
#include "generator.h"

void genbf_jump_statement(struct jump_statement *a)
{
    int i, bd;
    
    switch (a->type) {
        case _RETURN:
            /* FIXME: if there's anything after this, it's an error, but
             * there's no way to verify at present */
            if (a->has_expr) {
                genbf_expr(a->v._expr);
                /* this expression is the return address, so put it in place */
                bd = blockDepth() + tempstack;
                
                printf("[");
                for (i = 0; i < bd; i++)
                    printf("<<<<<");
                printf("+");
                for (i = 0; i < bd; i++)
                    printf(">>>>>");
                printf("-]");
                
                POP_TEMP;
                fflush(stdout);
            }
            break;
        
        /* case _GOTO:
            SPC; printf("goto\n");
            genbf_identifier(a->v._identifier);
            SPC; printf(";\n");
            break;
            
        case _CONTINUE:
            SPC; printf("continue;\n");
            break;
            
        case _BREAK:
            SPC; printf("break;\n");
            break;
            
        case _RETURN:
            SPC; printf("return\n");
            if (a->has_expr) {
                genbf_expr(a->v._expr);
            }
            SPC; printf(";\n");
            break; */
        
        default:
            UNIMPL("jump_statement");
    }
}
