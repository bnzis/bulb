/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_ENVIRONMENT_H
#define BULB_ENVIRONMENT_H
#include <stdio.h>
#include "core.h"
#include "hashmap.h"

bulbObj *bulbEnvGet(bulbEnv *env, char *symbol);

void bulbEnvSet(bulbEnv *env, char *symbol, bulbObj *newObj);

void bulbEnvDef(bulbEnv *env, char *symbol, bulbObj *obj);

void bulbEnvDelete(bulbEnv *env, char *symbol);

void bulbLoadModule(bulbEnv *env, bulbModule *module);

void bulb_err_variable_not_bound(char *name);

#endif /* BULB_ENVIRONMENT_H */
