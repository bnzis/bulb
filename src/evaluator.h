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
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include <stdio.h>
#include "types.h"
#include "hashmap.h"
#include "environment.h"

obj_t *eval(obj_t *ast, env_t *env);

obj_t *eval_sequence(obj_t *ast, env_t *env);

obj_t *eval_if(obj_t *ast, env_t *env);

obj_t *eval_define(obj_t *ast, env_t *env);

env_t *expand_env(obj_t *proc, obj_t *args, env_t *env);

bool not_a_keyword(char *sym);

void err_non_procedure(obj_t *proc);

void err_invalid_syntax(obj_t *tree);

void err_invalid_parameter_bool(obj_t *tree);

void err_invalid_len(unsigned expected, unsigned given);

#endif
