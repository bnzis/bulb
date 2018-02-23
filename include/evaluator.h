/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_EVALUATOR_H
#define BULB_EVALUATOR_H
#include <stdio.h>
#include "core.h"
#include "hashmap.h"
#include "environment.h"

bulbObj *bulbEval(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalSequence(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalIf(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalDefine(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalSet(bulbObj *ast, bulbEnv *env);

bulbEnv *bulbExpandEnv(bulbObj *ast, bulbObj *proc, bulbObj *args, bulbEnv *env);

bool bulbNotKeyword(char *sym);

void bulb_err_non_procedure(bulbObj *proc);

void bulb_err_invalid_syntax(bulbObj *tree);

void bulb_err_invalid_parameter_bool(bulbObj *tree);

void bulb_err_invalid_len(bulbObj *ast, unsigned expected, unsigned given);

#endif /* BULB_EVALUATOR_H */
