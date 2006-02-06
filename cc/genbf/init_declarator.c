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

void genbf_init_declarator(struct init_declarator *a)
{
    /* the tree should look like this:
       -init_declarator
       |
       |-declarator (v1)
       ||(FIXME: this may have a pointer, but that's not used yet)
       ||
       ||-declarator2 (v2)
       ||(FIXME: type must be _IDENTIFIER (no array support yet))
       ||
       ||-identifier (v1._identifier)
       |||v is the variable name
       */
    if (a->v1->v2->type != _IDENTIFIER) {
        ERROR("init_declarator", "Arrays and function declarations are not yet supported.");
    }
    
    /* push this variable */
    BF_PUSH;
    pushVar(a->v1->v2->v1._identifier->v, 1);
    
    if (a->assign) {
        UNIMPL("init_declarator with assignment");
    }
}
