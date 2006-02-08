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

void genbf_postfix_expr(struct postfix_expr *a)
{
    struct primary_expr *p_e;
    char *f;
    int i;
    
    switch (a->type) {
        case _PRIMARY_EXPR:
            genbf_primary_expr(a->v1._primary_expr);
            break;
            
        case _SIMPLE_FCALL:
        case _FCALL:
            /* for builtins, the tree should look like this:
               -postfix_expr
               |
               |-postfix_expr (v1._postfix_expr)
               ||(type must be _PRIMARY_EXPR)
               ||
               ||-primary_expr (v1._primary_expr)
               |||(type must be _IDENTIFIER)
               |||
               |||-identifier (v1._identifier)
               ||||v is the name of the function to call
               |
               |-argument_expr_list (v2._argument_expr_list) (_FCALL)
               ||(this contains the arguments)
               */
            if (a->v1._postfix_expr->type != _PRIMARY_EXPR)
                ERROR("postfix_expr", "Invalid function call!");
            
            p_e = a->v1._postfix_expr->v1._primary_expr;
            if (p_e->type != _IDENTIFIER)
                ERROR("postfix_expr", "Invalid function call!");
            
            f = p_e->v._identifier->v;
            
#include "builtin_functions.c"
            else {
                int topop = 0;
                
                /* this is a real function call */
                pushCall(f);
                if (a->type == _FCALL) {
                    topop = genbf_argument_expr_list(a->v2._argument_expr_list);
                    for (; topop > 0; topop--)
                        ignoreVar(); /* these aren't ours */
                }
                outBlock();
            }
            break;
            
        /* case _ARRAY_REF:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("[\n");
            genbf_expr(a->v2._expr);
            SPC; printf("]\n");
            break;
            
        case _SIMPLE_FCALL:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("()\n");
            break;
            
        case _FCALL:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("(\n");
            genbf_argument_expr_list(a->v2._argument_expr_list);
            SPC; printf(")\n");
            break;
            
        case _DOT:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf(".\n");
            genbf_identifier(a->v2._identifier);
            break;
            
        case _PTR_OP:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("->\n");
            genbf_identifier(a->v2._identifier);
            break;
            
        case _INC:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("++\n");
            break;
            
        case _DEC:
            genbf_postfix_expr(a->v1._postfix_expr);
            SPC; printf("--\n");
            break; */
            
        default:
            UNIMPL("postfix_expr");
    }
}

char *genbf_postfix_expr_get_primary(int type, struct postfix_expr *a)
{
    if (a->type != _PRIMARY_EXPR) return NULL;
    return genbf_primary_expr_get_primary(type, a->v1._primary_expr);
}
