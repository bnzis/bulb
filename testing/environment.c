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

bulbObj *bulbEnvGet(bulbEnv *env, char *symbol)
{
    bulbObj *obj = NULL;
    while (env != NULL && obj == NULL) {
        obj = bulbHashmapGet(env->local, symbol);
        env = env->upperEnv;
    }
    if (obj == NULL) bulb_err_variable_not_bound(symbol);
    return obj;
}

void bulbEnvSet(bulbEnv *env, char *symbol, bulbObj *obj)
{
    bulbHashmapSet(env->local, symbol, obj);
}

void bulbEnvDelete(bulbEnv *env, char *symbol)
{
    bulbHashmapDelete(env->local, symbol);
}

void bulbLoadModule(bulbEnv *env, bulbModule *module)
{
    unsigned i = 0;
    while (module->names[i]) {
        bulbObj *prim = (bulbObj*) malloc(sizeof(bulbObj));
        prim->type = BULB_PRIMITIVE;
        prim->data = module->primitives[i];
        bulbEnvSet(env, module->names[i], prim);
        i++;
    }
}

void bulb_err_variable_not_bound(char *name)
{
    printf("Exception: variable %s is not bound.\n", name);
    exit(1);
}
