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
/* Description
   ===========
   Environments are used as set of variables and procedures, so basically 
   everything declared in bulb is contained inside of an environment, (you can 
   find more details on environment.md):  
   `(def a 10) (def (func) (def b 20))`  
   The environment of func contains b, the upper 
   enviroment contains a and func. 

   bulbEnvGet
   ----------
   This function searches the variable inside the local hashmap, if it doesn't exist
   it tries in the upper environment and so on. */
bulbObj *bulbEnvGet(bulbEnv *env, char *symbol);

/* bulbEnvSet
   ----------
   This function corresponds to set! on bulb; it sets the variable to the new value, 
   if the variable doesn't exists in the local hashmap it tries in the upper environment
   and so on.
   ```(def a 10)
    (def (f) (set! a 15))
    (f)
    a ;=> 15 ``` */
void bulbEnvSet(bulbEnv *env, char *symbol, bulbObj *newObj);

/* bulbEnvDef
   ----------
   This function corresponds to def on bulb; it define the variable inside the
   environment. */
void bulbEnvDef(bulbEnv *env, char *symbol, bulbObj *obj);

/* bulbEnvDelete
   -------------
   Deletes the variable inside the environment, for the moment it is not used. */
void bulbEnvDelete(bulbEnv *env, char *symbol);

/* bulbLoadModule
   --------------
   Loads the module inside the environment. */
void bulbLoadModule(bulbEnv *env, bulbModule *module);

/* Exceptions
   ---------- */
void bulb_err_variable_not_bound(char *name);

#endif /* BULB_ENVIRONMENT_H */
