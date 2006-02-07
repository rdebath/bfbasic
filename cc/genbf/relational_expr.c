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
    if (a->type == _SHIFT_EXPR) {
        genbf_shift_expr(a->v1._shift_expr);
        return;
    }
    
    /* push the two operands */
    genbf_relational_expr(a->v1._relational_expr);
    genbf_shift_expr(a->v2);
    
    /* and do the comparison */
    switch (a->type) {
        case _LESS:
            /* less is just a <= (b - 1) */
            printf("-");
        case _LE:
            /* FIXME: THIS IS THE ONLY TESTED RELATIONAL EXPRESSION! */
            /* layout:
               O1 . . points temp *O2 . . temp temp . . . temp temp
               A point is given if O1 == O2 or if O1 < O2 */
            printf("[>>>+>+<<<<-]>>>[<<<+>>>-]<<<"
                   /* O1 . . points 0 *O2 . . 0 O2 */
                   "<<<<<[>>>>+>>>>+<<<<<<<<-]>>>>[<<<<+>>>>-]>>>>>"
                   /* O1 . . points 0 O2 . . O1 *O2 */
                   "[-<-[>>>>>+>+<<<<<<-]>>>>>>[<<<<<<+>>>>>>-]<<<<<<"
                   "<<<<+>>>>[[-]<<<<->>>>]>>>>>[<<<<<+>>>>>-]<<<<<"
                   "<<<<[<+>-]>>>>>]"
                   /* O1 . . points 0 O2 . . ? *0 */
                   "<[-]<<<<<"
                   "<<<[-]>>>[<<<+>>>-]>>");
            POP_TEMP;
            fflush(stdout);
            
            break;
            
        case _GREATER:
            /* greater is just a >= (b + 1) */
            printf("+");
        case _GE:
            printf("[>>>+>+<<<<-]>>>[<<<+>>>-]<<<"
                   /* O1 . . points 0 *O2 . . 0 O2 */
                   "<<<<<[>>>>+>>>>+<<<<<<<<-]>>>>[<<<<+>>>>-]>>>>"
                   /* O1 . . points 0 O2 . . *O1 O2 */
                   "[->-[>>>>+>+<<<<<-]>>>>>[<<<<<+>>>>>-]<<<<<"
                   "<<<<<+>>>>>[[-]<<<<<->>>>>]>>>>[<<<<+>>>>-]<<<<"
                   "<<<<<[<+>-]>>>>]"
                   /* O1 . . points 0 O2 . . ? *0 */
                   ">[-]<<<<<<"
                   "<<<[-]>>>[<<<+>>>-]>>");
            POP_TEMP;
            fflush(stdout);
            break;
    }
}

char *genbf_relational_expr_get_primary(int type, struct relational_expr *a)
{
    if (a->type != _SHIFT_EXPR) return NULL;
    return genbf_shift_expr_get_primary(type, a->v1._shift_expr);
}
