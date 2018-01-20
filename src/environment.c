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
#include "environment.h"

obj_t *env_get(env_t *env, char *symbol)
{
    obj_t *obj = NULL;
    while (env != NULL && obj == NULL) {
        obj = get(env->local, symbol);
        env = env->upper_level;
    }
    if (obj == NULL) err_variable_not_bound(symbol);
    return obj;
}

void env_set(env_t *env, char *symbol, obj_t *obj)
{
    set(env->local, symbol, obj);
}

void env_delete(env_t *env, char *symbol)
{
    delete(env->local, symbol);
}

void err_variable_not_bound(char *name)
{
    printf("Exception: variable %s is not bound.\n", name);
    exit(1);
}
