/*
    Bulb - the Lisp Interpreter
    Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

bulbEnv *bulbExpandEnv(bulbObj *proc, bulbObj *args, bulbEnv *env);

bool bulbNotKeyword(char *sym);

void bulb_err_non_procedure(bulbObj *proc);

void bulb_err_invalid_syntax(bulbObj *tree);

void bulb_err_invalid_parameter_bool(bulbObj *tree);

void bulb_err_invalid_len(unsigned expected, unsigned given);

#endif /* BULB_EVALUATOR_H */
