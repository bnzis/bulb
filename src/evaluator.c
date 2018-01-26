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
    while (true)
        if (ast->type == CONS) {
            unsigned cdr_type = cdr(ast)->type;
            if (ast->cons.cdr != NULL && cdr_type != NIL) {
                eval(car(ast), env);
                ast = cdr(ast); 
            } else  
                return eval(car(ast), env);
        } else 
            return eval(ast, env);
}

obj_t *eval_args(obj_t *ast, env_t *env)
{
    if (ast == NULL) return NULL;
    obj_t *args = malloc(sizeof(obj_t));
    obj_t *front = args;
    args->type = CONS;
    if (ast->type != CONS)
        return eval(ast, env);
    while (ast != NULL) {
        free(car(front));
        obj_t *val = eval(car(ast), env);
        set_car(front, val);
        set_cdr(front, malloc(sizeof(obj_t)));
        front = cdr(front);
        front->type = CONS;
        ast = cdr(ast);
    }
    return args;
}

obj_t *eval_define(obj_t *ast, env_t *env)
{
    obj_t *val;
    char *sym;
    unsigned len = list_len(ast);
    if (len < 2) err_invalid_syntax(ast);
    if (cadr(ast)->type == SYMBOL) {
        sym = cadr(ast)->symbol.buff;
        val = eval(caddr(ast), env);
        env_set(env, sym, val);
    } else if (cadr(ast)->type == CONS) {
        sym = caadr(ast)->symbol.buff;
        if (!not_a_keyword(sym)) err_invalid_syntax(ast);
        obj_t *args = cdr(cadr(ast));
        obj_t *body = cdr(cdr(ast));
        val = malloc(sizeof(obj_t));
        val->type = PROCEDURE;
        val->procedure.args = args;
        val->procedure.body = body;
        val->procedure.env = env;
        env_set(env, sym, val);
    } else 
        err_invalid_syntax(ast);
    return val;
}

obj_t *eval_if(obj_t *ast, env_t *env)
{
    obj_t *tmp;
    unsigned len = list_len(ast);
    if (len < 3 || len > 4) err_invalid_syntax(ast);
    if (eval(cadr(ast), env)->boolean == true) tmp = caddr(ast);
    else if (len == 3) {
        tmp = malloc(sizeof(obj_t));
        tmp->type = NIL;
    } else
        tmp = cadddr(ast);
    return tmp;
}

obj_t *eval_lambda(obj_t *ast, env_t *env)
{
    obj_t *proc = malloc(sizeof(obj_t));
    proc->type = PROCEDURE;
    proc->procedure.args = car(ast);
    proc->procedure.body = cdr(ast);
    proc->procedure.env = env;
    return proc;
}

obj_t *eval(obj_t *ast, env_t *env)
{
    if (!ast) {
        obj_t *tmp = malloc(sizeof(obj_t));
        tmp->type = NIL;
        return tmp;
    }
    while (true)
        switch(ast->type) {
            case STRING: case INT: case FLOAT: case BOOL: case NIL:
                return ast;
            case PROCEDURE:
                return ast;
            case SYMBOL:
                return env_get(env, ast->symbol.buff);
            case CONS:
                if (car(ast) == NULL) return NULL;
                if (car(ast)->type == SYMBOL) {
                    char *op = car(ast)->symbol.buff;
                    if (strcmp(op, "def") == 0)
                        return eval_define(ast, env);
                    else if (strcmp(op, "if") == 0) {
                        ast = eval_if(ast, env);
                        break;
                    } else if (strcmp(op, "lambda") == 0) {
                        return eval_lambda(cdr(ast), env);
                    } else if (strcmp(op, "begin") == 0)
                        return eval_sequence(cdr(ast), env);
                    else if(strcmp(op, "qu") == 0)
                        if (list_len(cdr(ast)) == 1) return cdr(ast);
                        else err_invalid_syntax(ast);
                }
                obj_t *proc = eval(car(ast), env);
                obj_t *args = eval_args(cdr(ast), env);
                if (proc->type == PRIMITIVE) {
                    return (proc->primitive)(args, env);
                } else if (proc->type == PROCEDURE) {
                    env_t *new_env = expand_env(proc, args, proc->procedure.env);
                    ast = proc->procedure.body;
                    return eval_sequence(ast, new_env);
                } else
                    err_non_procedure(proc);
    }
}


env_t *expand_env(obj_t *obj, obj_t *args, env_t *upper_level)
{
    if (args->type == NIL) return upper_level;
    if (obj->type != PROCEDURE) err_non_procedure(obj);
    proc_t *proc = &obj->procedure;
    unsigned params_len = list_len(proc->args), args_len = list_len(args) - 1;
    if (params_len != args_len)
        err_invalid_len(params_len, args_len);
    env_t *new_env = malloc(sizeof(env_t));
    new_env->local = malloc(sizeof(hashmap_t));
    new_env->local->data = malloc(sizeof(obj_t*) * HMAP_ROWS);
    new_env->upper_level = upper_level;
    unsigned i = 0;
    obj_t *tmp = proc->args;
    while (i < params_len) {
        env_set(new_env, car(tmp)->symbol.buff, car(args));
        tmp = cdr(tmp);
        args = cdr(args);
        i++;
    }
    return new_env;
}

bool not_a_keyword(char *symbol)
{
    return (strcmp(symbol, "def") != 0 &&
               strcmp(symbol, "lambda") != 0 &&
               strcmp(symbol, "if") != 0 &&
               strcmp(symbol, "begin") != 0 &&
               strcmp(symbol, "qu") != 0);
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
    printf("Exception: invalid syntax: (");
    print_ast(tree);
    printf(").\n");
    exit(1);
}

void err_invalid_len(unsigned expected, unsigned given)
{
    printf("Exception: incorrect number of arguments (expected %d, given %d).\n", 
            expected, given);
    exit(1);
}
