/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "evaluator.h"

bulbObj *bulbEvalSequence(bulbObj *ast, bulbEnv *env) 
{
    while (true)
        if (ast->type == BULB_CONS) {
            if (bulbGetCdr(ast) != bulbNil) {
                bulbEval(bulbGetCar(ast), env);
                ast = bulbGetCdr(ast); 
            } else  
                return bulbEval(bulbGetCar(ast), env);
        } else 
            return bulbEval(ast, env);
}

bulbObj *bulbEvalArgs(bulbObj *ast, bulbEnv *env)
{
    bulbObj *args = bulbNewConsObj(
    (bulbObj*) malloc(sizeof(bulbObj)), 
    (bulbObj*) malloc(sizeof(bulbObj)));
    bulbObj **front = &args;
    while (ast != bulbNil) {
        bulbObj *val = bulbEval(bulbGetCar(ast), env);
        bulbSetCar(*front, val);
        bulbSetCdr(*front, bulbNewConsObj(
        (bulbObj*) malloc(sizeof(bulbObj)), 
        (bulbObj*) malloc(sizeof(bulbObj))));
        front = &bulbMakeCons(*front)->cdr;
        ast = bulbGetCdr(ast);
    }
    free(*front);
    *front = bulbNil;
    return args;
}

bulbObj *bulbEvalDefine(bulbObj *ast, bulbEnv *env)
{
    bulbObj *val = bulbNil;
    char *sym;
    unsigned len = bulbListLen(ast);
    if (len < 2) bulb_err_invalid_syntax(ast);
    if (bulbGetCadr(ast)->type == BULB_SYMBOL) {
        sym = bulbGetSymbolText(bulbGetCadr(ast));
        val = bulbEval(bulbGetCaddr(ast), env);
        bulbEnvDef(env, sym, val);
    } else if (bulbGetCadr(ast)->type == BULB_CONS) {
        sym = bulbGetSymbolText(bulbGetCaadr(ast));
        if (!bulbNotKeyword(sym)) bulb_err_invalid_syntax(ast);
        bulbObj *args = bulbGetCdr(bulbGetCadr(ast));
        bulbObj *body = bulbGetCdr(bulbGetCdr(ast));
        val = bulbNewProcObj(args, body, env);
        bulbEnvDef(env, sym, val);
    } else 
        bulb_err_invalid_syntax(ast);
    return val;
}

bulbObj *bulbEvalSet(bulbObj *ast, bulbEnv *env)
{
    bulbObj *val = bulbNil;
    char *sym;
    unsigned len = bulbListLen(ast);
    if (len < 2) bulb_err_invalid_syntax(ast);
    if (bulbGetCadr(ast)->type == BULB_SYMBOL) {
        sym = bulbGetSymbolText(bulbGetCadr(ast));
        val = bulbEval(bulbGetCaddr(ast), env);
        bulbEnvSet(env, sym, val);
    } else if (bulbGetCadr(ast)->type == BULB_CONS) {
        sym = bulbGetSymbolText(bulbGetCaadr(ast));
        if (!bulbNotKeyword(sym)) bulb_err_invalid_syntax(ast);
        bulbObj *args = bulbGetCdr(bulbGetCadr(ast));
        bulbObj *body = bulbGetCdr(bulbGetCdr(ast));
        val = bulbNewProcObj(args, body, env);
        bulbEnvSet(env, sym, val);
    } else 
        bulb_err_invalid_syntax(ast);
    return val;
}

bulbObj *bulbEvalIf(bulbObj *ast, bulbEnv *env)
{
    bulbObj *out;
    unsigned len = bulbListLen(ast);
    if (len < 3 || len > 4) bulb_err_invalid_syntax(ast);
    if (bulbEval(bulbGetCadr(ast), env) == bulbTrue) 
        out = bulbGetCaddr(ast);
    else if (len == 3) out = bulbNil;
    else out = bulbGetCadr(bulbGetCddr(ast));
    return out;
}

bulbObj *bulbEvalLambda(bulbObj *ast, bulbEnv *env)
{
    return bulbNewProcObj(bulbGetCar(ast), bulbGetCdr(ast), env);
}

bulbObj *bulbEval(bulbObj *ast, bulbEnv *env)
{
    if (!ast)
        return bulbNil;
cicle:
    while (true)
        if(ast->type == BULB_CONS) {
            if (bulbGetCar(ast) == NULL) return bulbNil;
            if (bulbGetCar(ast)->type == BULB_SYMBOL) {
                char *op = bulbGetSymbolText(bulbGetCar(ast));
                if (strcmp(op, "def") == 0)
                    return bulbEvalDefine(ast, env);
                else if (strcmp(op, "set!") == 0)
                    return bulbEvalSet(ast, env);
                else if (strcmp(op, "if") == 0) {
                    ast = bulbEvalIf(ast, env);
                    goto cicle;
                } else if (strcmp(op, "lambda") == 0)
                    return bulbEvalLambda(bulbGetCdr(ast), env);
                else if (strcmp(op, "begin") == 0)
                    return bulbEvalSequence(bulbGetCdr(ast), env);
                else if(strcmp(op, "qu") == 0) {
                    if (bulbListLen(bulbGetCdr(ast)) == 1) {
                        return bulbGetCadr(ast);
                    } else bulb_err_invalid_syntax(ast);
                }
            }
            bulbObj *proc = bulbEval(bulbGetCar(ast), env);
            bulbObj *args = bulbEvalArgs(bulbGetCdr(ast), env);
            if (proc->type == BULB_PRIMITIVE) {
                return ((bulbPrimitive) proc->data)(args, env);
            } else if (proc->type == BULB_PROCEDURE) {
                bulbEnv *newEnv = bulbExpandEnv(bulbGetCar(ast), proc, 
                args, bulbGetProcEnv(proc));
                ast = bulbGetProcBody(proc);
                return bulbEvalSequence(ast, newEnv);
            } else
                bulb_err_non_procedure(proc);
        } else if (ast->type == BULB_SYMBOL) {
            return bulbEnvGet(env, bulbGetSymbolText(ast));
        } else return ast;
}


bulbEnv *bulbExpandEnv(bulbObj *ast, bulbObj *obj, bulbObj *args, bulbEnv *upperEnv)
{
    if (args == bulbNil) return upperEnv;
    if (obj->type != BULB_PROCEDURE) bulb_err_non_procedure(obj);
    unsigned paramsLen = bulbListLen(bulbGetProcArgs(obj)), 
             argsLen = bulbListLen(args);
    if (paramsLen != argsLen)
        bulb_err_invalid_len(ast, paramsLen, argsLen);
    bulbEnv *newEnv = bulbNewEnv(upperEnv);
    unsigned i = 0;
    bulbObj *tmp = bulbGetProcArgs(obj);
    while (i < paramsLen) {
        bulbEnvSet(newEnv, bulbGetSymbolText(bulbGetCar(tmp)), bulbGetCar(args));
        tmp = bulbGetCdr(tmp);
        args = bulbGetCdr(args);
        i++;
    }
    return newEnv;
}

bool bulbNotKeyword(char *symbol)
{
    return (strcmp(symbol, "def") != 0 &&
               strcmp(symbol, "lambda") != 0 &&
               strcmp(symbol, "if") != 0 &&
               strcmp(symbol, "begin") != 0 &&
               strcmp(symbol, "qu") != 0);
}

void bulb_err_non_procedure(bulbObj *proc)
{
    printf("Exception: attemp to apply non-procedure ");
    bulbPrintAst(proc);
    printf(".\n");
    exit(1);
}

void bulb_err_invalid_syntax(bulbObj *tree)
{
    printf("Exception: invalid syntax: (");
    bulbPrintAst(tree);
    printf(").\n");
    exit(1);
}

void bulb_err_invalid_len(bulbObj *ast, unsigned expected, unsigned given)
{
    printf("Exception: ");
    bulbPrintAst(ast);
    printf(": incorrect number of arguments (expected %d, given %d).\n", 
            expected, given);
    exit(1);
}
