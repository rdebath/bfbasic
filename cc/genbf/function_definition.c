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

#include <stdlib.h>

#include "../genbf.h"
#include "generator.h"

void genbf_function_definition(struct function_definition *a)
{
    struct declarator2 *decla, *declb;
    
    /* FIXME: this should store a list of declared functions and refer to that
     * for function calls */
    
    /* the tree here should look like this:
       -function_definition
       |-declarator (v2) (guaranteed)
       ||
       ||-declarator2 (v2) (guaranteed)
       |||(type must be _SIMPLE_FUNC or _FUNC_BY_TYPE)
       |||
       |||-declarator2 (v1._declarator2)
       ||||(type must be _IDENTIFIER)
       ||||
       ||||-identifier (v1._identifier)
       |||||v is the function name
       |||
       |||-parameter_identifier_list (v2._parameter_type_list) (_FUNC)
       ||||these must be pushed to stack
       |
       |-function_body (v3)
       */
    
    decla = a->v2->v2;
    /* decla must have type _SIMPLE_FUNC or _FUNC_BY_TYPE */
    if (decla->type != _SIMPLE_FUNC && decla->type != _FUNC_BY_TYPE) {
        fprintf(stderr, "Invalid function definition!\n");
        exit(1);
    }
    
    declb = decla->v1._declarator2;
    /* declb must have type _IDENTIFIER */
    if (declb->type != _IDENTIFIER) {
        fprintf(stderr, "Invalid function definition!\n");
        exit(1);
    }
    
    /* now we can start the block */
    pushNamedBlock(declb->v1._identifier->v);
    outBlock();
    
    /* and push all the parameter_identifier_list variables on */
    if (decla->type != _SIMPLE_FUNC)
        genbf_parameter_type_list(decla->v2._parameter_type_list);
    
    genbf_function_body(a->v3);
}
