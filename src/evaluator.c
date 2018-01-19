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

obj_t *eval_sequence(obj_t *ast, env_t *env) 
{
    if (ast->type == CONS) {
        unsigned cdr_type = cdr(ast)->type;
        if (ast->data.cons.cdr != NULL && cdr_type != NIL) {
            eval(car(ast), env);
            return eval_sequence(cdr(ast), env); 
        } else 
            return eval(car(ast), env);
    } else 
        return eval(ast, env);
}

obj_t *eval_args(obj_t *ast, env_t *env)
{
    obj_t *args = malloc(sizeof(obj_t));
    obj_t *front = args;
    args->type = CONS;
    if (ast->type != CONS)
        return eval(ast, env);
    while (ast != NULL) {
        free(car(front));
        set_car(front, eval(car(ast), env));
        set_cdr(front, malloc(sizeof(obj_t)));
        front = cdr(front);
        front->type = CONS;
        ast = cdr(ast);
    }
    free(front);
    return args;
}

obj_t *eval_define(obj_t *ast, env_t *env)
{
    obj_t *val;
    char *sym;
    if (cadr(ast)->type == SYMBOL) {
        if (list_len(ast) != 3) err_invalid_syntax(ast);
        sym = cadr(ast)->data.symbol.buff;
        val = eval(caddr(ast), env);
        env_set(env, sym, val);
    } else if (cadr(ast)->type == CONS) {
        sym = caadr(ast)->data.symbol.buff;
        obj_t *args = cdr(cadr(ast));
        obj_t *body = cdr(cdr(ast));
        val = malloc(sizeof(obj_t));
        val->type = PROCEDURE;
        val->data.procedure.args = args;
        val->data.procedure.body = body;
        env_set(env, sym, val);
    }
    
    return val;
}

obj_t *eval_if(obj_t *ast, env_t *env)
{
    obj_t *tmp;
    if (eval(cadr(ast), env)->data.boolean == true) tmp = caddr(ast);
    else tmp = cadddr(ast);
    return eval(tmp, env);
}

obj_t *eval(obj_t *ast, env_t *env)
{
    if (!ast) return NULL;
    switch(ast->type) {
        case STRING: case INT: case FLOAT: case PROCEDURE: case BOOL: case NIL:
            return ast;
        case SYMBOL:
                return env_get(env, ast->data.symbol.buff);
        case CONS:
            if (car(ast) == NULL) return NULL;
            if (car(ast)->type == SYMBOL) {
                char *op = car(ast)->data.symbol.buff;
                if (strcmp(op, "def") == 0)
                    return eval_define(ast, env);
                else if (strcmp(op, "if") == 0) {
                    return eval_if(ast, env);
                } else if (strcmp(op, "lambda") == 0) {
                    /* LAMBDA */
                } else if (strcmp(op, "begin") == 0) 
                    return eval_sequence(cdr(ast), env);
                else if(strcmp(op, "quote") == 0)
                    if (list_len(cdr(ast)) == 1) return cdr(ast);
                    else err_invalid_syntax(ast);
            }
            return apply(ast, env);
    }
}

obj_t *apply(obj_t *ast, env_t *env)
{
    obj_t *proc = eval(car(ast), env);
    obj_t *args = eval_args(cdr(ast), env);
    if (proc->type == PRIMITIVE) {
        return (proc->data.primitive)(args, env);
    } else if (proc->type == PROCEDURE) {
        /* to implement */
    } else 
        err_non_procedure(proc);
}

void err_non_procedure(obj_t *proc)
{
    printf("Exception: attemp to apply non-procedure ");
    print_ast(proc);
    printf(".\n");
    exit(1);
}

void err_invalid_syntax(obj_t *tree)
{
    printf("Exception: invalid syntax:\n(");
    print_ast(tree);
    printf(")\n");
    exit(1);
}
