/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016, Yann Collet (xxhash)
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include "bulb.h"

void print_ast(obj_t *tree)
{
    if (tree) {
        if (tree->type == SYMBOL || tree->type == STRING)
            printf("%s\n", tree->data.symbol.buff);
        else if (tree->type == INT)
            printf("%i\n", tree->data.integer);
        else if (tree->type == FLOAT)
            printf("%f\n", tree->data.floating);
        else if(tree->type == CONS) {
            print_ast(tree->data.cons.car);
            print_ast(tree->data.cons.cdr);
        }
    }
}

int main() 
{
    char *program = "(+ 10 20)";
    obj_t *tree = parse(program);
    env_t *env = malloc(sizeof(env_t));
    env->local = malloc(sizeof(hashmap_t));
    env->local->data = malloc(sizeof(obj_t*) * HMAP_ROWS);
    obj_t module = atom("core", strlen("core"));
    load_module(env, &module);
    printf("%d\n", eval_sequence(tree, env)->data.integer);
    return 0;
}
