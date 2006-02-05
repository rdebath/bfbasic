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
void genbf_declarator2(struct declarator2 *a)
{ UNIMPL("declarator2"); }

/* this function allows you to take a whole declarator2 tree and just find the
 * identifier from it */
char *genbf_declarator2_get_identifier(struct declarator2 *a)
{
    switch (a->type) {
        case _IDENTIFIER:
            return a->v1._identifier; /* here it is! */
            
        case _DECLARATOR:
            /* we need to recurse through this */
            return genbf_declarator2_get_identifier(a->v1._declarator->v2);
            
        default:
            return genbf_declarator2_get_identifier(a->v1._declarator2);
    }
}
