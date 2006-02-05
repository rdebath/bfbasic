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

void genbf_relational_expr(struct relational_expr *a)
{
    switch (a->type) {
        case _SHIFT_EXPR:
            genbf_shift_expr(a->v1._shift_expr);
            break;
            
        /* case _LESS:
            genbf_relational_expr(a->v1._relational_expr);
            SPC; printf("<\n");
            genbf_shift_expr(a->v2);
            break;
            
        case _GREATER:
            genbf_relational_expr(a->v1._relational_expr);
            SPC; printf(">\n");
            genbf_shift_expr(a->v2);
            break;
            
        case _LE:
            genbf_relational_expr(a->v1._relational_expr);
            SPC; printf("<=\n");
            genbf_shift_expr(a->v2);
            break;
            
        case _GE:
            genbf_relational_expr(a->v1._relational_expr);
            SPC; printf(">=\n");
            genbf_shift_expr(a->v2);
            break; */
            
        default:
            UNIMPL("relational_expr");
    }
}

char *genbf_relational_expr_get_primary(int type, struct relational_expr *a)
{
    if (a->type != _SHIFT_EXPR) return NULL;
    return genbf_shift_expr_get_primary(type, a->v1._shift_expr);
}
