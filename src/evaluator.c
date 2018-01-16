/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016 Yann Collet (xxhash)
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
#include "evaluator.h"
#include <stdio.h>

obj_t *eval_sequence(obj_t *ast, env_t *env) 
{
    if (ast->type == CONS) {
        unsigned cdr_type = ast->data.cons.cdr->type;
        if (ast->data.cons.cdr != NULL && cdr_type != NIL) {
            eval(ast->data.cons.car, env);
            return eval_sequence(ast->data.cons.cdr, env); 
        } else 
            return eval(ast->data.cons.car, env);
    } else 
        return eval(ast, env);
}

obj_t *eval_define(obj_t *ast, env_t *env)
{
    obj_t *val;
    if (ast->data.cons.cdr->data.cons.car->type == SYMBOL) {
        char *sym = ast->data.cons.cdr->data.cons.car->data.symbol.buff;
        val = ast->data.cons.cdr->data.cons.cdr->data.cons.car;
        val = eval(val, env);
        env_set(env, sym, val);
    }
    /* if procedure */
    return val;
}

obj_t *eval_if(obj_t *ast, env_t *env)
{
    if (eval(ast->data.cons.cdr->data.cons.car, env)->data.boolean == true) { 
        obj_t *out = ast->data.cons.cdr->data.cons.cdr->data.cons.car;
        return eval(out, env);
    } else {
        obj_t *out = ast->data.cons.cdr->data.cons.cdr->data.cons.cdr->data.cons.car;
        return eval(out, env);
    }
}

obj_t *eval(obj_t *ast, env_t *env)
{
    if (!ast) return NULL;
    switch(ast->type) {
        case STRING: case INT: case FLOAT: case PROCEDURE: case BOOL:
            return ast;
        case SYMBOL:
            return env_get(env, ast->data.symbol.buff);
        case CONS:
            if (ast->data.cons.car == NULL) return NULL;
            if (ast->data.cons.car->type == SYMBOL) {
                char *op = ast->data.cons.car->data.symbol.buff;
                if (strcmp(op, "def") == 0)
                    return eval_define(ast, env);
                else if (strcmp(op, "if") == 0) {
                    return eval_if(ast, env);
                } else if (strcmp(op, "lambda") == 0) {
                    /* LAMBDA */
                } else if (strcmp(op, "begin") == 0) 
                    return eval_sequence(ast->data.cons.cdr, env);
            }
            /* return apply(ast, env); */
    }
}
