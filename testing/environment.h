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
#ifndef BULB_ENVIRONMENT_H
#define BULB_ENVIRONMENT_H
#include <stdio.h>
#include "core.h"
#include "hashmap.h"

bulbObj *bulbEnvGet(bulbEnv *env, char *symbol);

void bulbEnvSet(bulbEnv *env, char *symbol, bulbObj *obj);

void bulbEnvDelete(bulbEnv *env, char *symbol);

void bulbLoadModule(bulbEnv *env, bulbModule *module);

void bulb_err_variable_not_bound(char *name);

#endif /* BULB_ENVIRONMENT_H */
