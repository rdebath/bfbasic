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

void genbf_unary_expr(struct unary_expr *a)
{
    switch (a->type) {
        case _POSTFIX_EXPR:
            genbf_postfix_expr(a->v1._postfix_expr);
            break;
            
        /* case _INC:
            SPC; printf("++\n");
            genbf_unary_expr(a->v1._unary_expr);
            break;
            
        case _DEC:
            SPC; printf("--\n");
            genbf_unary_expr(a->v1._unary_expr);
            break;
            
        case _UNARY_OP:
            genbf_unary_operator(a->v1._unary_operator);
            genbf_cast_expr(a->v2);
            break;
            
        case _SIZEOF_EXPR:
            SPC; printf("sizeof\n");
            genbf_unary_expr(a->v1._unary_expr);
            break;
            
        case _SIZEOF_TYPE:
            SPC; printf("sizeof\n");
            genbf_type_name(a->v1._type_name);
            break; */
            
        default:
            UNIMPL("unary_expr");
    }
}

char *genbf_unary_expr_get_primary(int type, struct unary_expr *a)
{
    if (a->type != _POSTFIX_EXPR) return NULL;
    return genbf_postfix_expr_get_primary(type, a->v1._postfix_expr);
}
