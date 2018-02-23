/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
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

void bulbEnvSet(bulbEnv *env, char *symbol, bulbObj *newObj)
{
    bulbObj *obj = NULL;
    while (env->upperEnv != NULL && obj == NULL) {
        obj = bulbHashmapGet(env->local, symbol);
        env = env->upperEnv;
    }
    bulbHashmapSet(env->local, symbol, newObj);
}

void bulbEnvDef(bulbEnv *env, char *symbol, bulbObj *obj)
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
        prim->data = (bulbPrimitive*) module->primitives[i];
        bulbEnvDef(env, module->names[i], prim);
        i++;
    }
}

void bulb_err_variable_not_bound(char *name)
{
    printf("Exception: variable %s is not bound.\n", name);
    exit(1);
}
