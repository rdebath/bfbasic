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

#include <string.h>

#include "../genbf.h"
#include "generator.h"

void genbf_selection_statement(struct selection_statement *a)
{
    char *nname;
    
    switch (a->type) {
        case _IF:
        case _IF_ELSE:
            genbf_expr(a->v1);
            
            /* this will use a sneaky "subblock" format to make the jump-back
             * location predictable.  basically:
             * main:
             *  if (blah) {
             *   main!0!1
             *  } else {
             *   main!1!1
             *  }
             * main!2
             */
            
            /* get an "if-not" as well */
            printf("[>>>+>+<<<<-]>>>>[<<<<+>>>>-]+"
                   "<[[-]>-<<<<(%s!%d!1)>>>]"
                   ">[-<<<<(%s!%d%s)>>>>]"
                   "<<<<",
                   curblock->name, curblock->num + 1,
                   curblock->name, curblock->num + 2,
                   a->type == _IF_ELSE ? "!1" : "");
            
            POP_TEMP;
            
            /* go on to the if-block */
            pushBlock();
            /* nname contains the new multi-! name */
            nname = (char *) malloc(strlen(curblock->name) + 11);
            if (!nname) { perror("malloc"); exit(1); }
            /* this num is what makes this a "subblock" */
            sprintf(nname, "%s!%d", curblock->name, curblock->num);
            free(curblock->name);
            curblock->name = nname;
            curblock->num = 1;
            outBlock();
            
            genbf_statement(a->v2);
            
            /* this needs to continue to the proper place */
            if (a->type == _IF) {
                printf("(%s!%d)", curblock->next->name, curblock->next->num + 2);
            } else {
                printf("(%s!%d)", curblock->next->name, curblock->next->num + 3);
                
                /* this is an if/else, so now we need yet another subblock */
                popBlock();
                pushBlock();
                /* nname contains the new multi-! name */
                nname = (char *) malloc(strlen(curblock->name) + 11);
                if (!nname) { perror("malloc"); exit(1); }
                /* this num is what makes this a "subblock" */
                sprintf(nname, "%s!%d", curblock->name, curblock->num + 1);
                free(curblock->name);
                curblock->name = nname;
                curblock->num = 1;
                outBlock();
                
                genbf_statement(a->v3);
                
                printf("(%s!%d)", curblock->next->name, curblock->next->num + 3);
            }
            
            /* finally continue with our regularly scheduled programming */
            popBlock();
            pushBlock();
            if (a->type == _IF) {
                curblock->num += 1;
            } else {
                curblock->num += 2;
            }
            outBlock();
            
            break;
            
        case _SWITCH:
            UNIMPL("selection_statement");
            /* SPC; printf("switch (\n");
            genbf_expr(a->v1);
            SPC; printf(")\n");
            genbf_statement(a->v2); */
            break;
    }
}
