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

void genbf_assignment_expr(struct assignment_expr *a)
{
    char *ident;
    int i, d;
    
    switch (a->type) {
        case _CONDITIONAL_EXPR:
            genbf_conditional_expr(a->v1._conditional_expr);
            break;
            
        case _ASSIGNMENT:
            /* FIXME: only primitive assignments are supported thusfar */
            ident = genbf_unary_expr_get_primary(_IDENTIFIER, a->v1._unary_expr);
            if (!ident)
                ERROR("assignment_expr", "Bad or unsupported assignment!");
            
            /* FIXME: only primitive (=) assignments :) */
            if (a->v2->type != _ASSIGN)
                ERROR("assignment_expr", "Only primitive assignments are supported thusfar.");
            
            genbf_assignment_expr(a->v3);
            
            /* now we have a value on top of the stack that we want to put in
             * the variable */
            d = varDepth(ident);
            if (d == -1)
                ERROR("assignment_expr", "Bad or uninitialized variable.");
            
            /* carry this value ... */
            printf("[>>>>+<<<<-]>>>>");
            
            /* the proper distance */
            printf("[");
            for (i = 0; i < d; i++)
                printf("<<<<<");
            printf("+");
            for (i = 0; i < d; i++)
                printf(">>>>>");
            printf("-]");
            for (i = 0; i < d; i++)
                printf("<<<<<");
            /* then put it in place */
            printf("<<<<[-]>>>>[<<<<+>>>>-]<<<<");
            /* and come back */
            for (i = 0; i < d; i++)
                printf(">>>>>");
            
            fflush(stdout);
            
            /* leave the value of the expression to pass up */
            
            /*genbf_unary_expr(a->v1._unary_expr);
            genbf_assignment_operator(a->v2);
            genbf_assignment_expr(a->v3);
            UNIMPL("assignment_expr");*/
            break;
    }
}

char *genbf_assignment_expr_get_primary(int type, struct assignment_expr *a)
{
    if (a->type != _CONDITIONAL_EXPR) return NULL;
    return genbf_conditional_expr_get_primary(type, a->v1._conditional_expr);
}
