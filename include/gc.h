/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_GC_H
#define BULB_GC_H
#include "core.h"
#include "environment.h"

/* bulbObjPool
   -----------
   A list which contains all the allocated objects. */
extern bulbObj *bulbObjPool;
extern bulbObj *bulbObjPoolFront;

/* bulbNewObj 
   ---------- 
   Allocates a new object and insert it to the pool for tracking. */
bulbObj *bulbNewObj();

/* bulbGCMarkObj
   -------------
   Marks the object, recursively. */
void bulbGCMarkObj(bulbObj *obj);

/* bulbGCMarkEnv
   -------------
   Marks all the objects inside the given environment. */
void bulbGCMarkEnv(bulbEnv *env);

/* bulbGCSweep
   -----------
   Remove garbage, the given environment **MUST** be the global environment. */
void bulbGCSweep();

/* bulbNewConsObj,  bulbNewStringObj ...
   -------------------------------------
   Creating a new object is a bit mechanical, I prefer to use these functions 
   to make the code more clear and readable. */
bulbObj *bulbNewConsObj(bulbObj *car, bulbObj *cdr);

bulbObj *bulbNewStringObj(char *text, unsigned len);

bulbObj *bulbNewProcObj(bulbObj *args, bulbObj *body, bulbEnv *env);

bulbEnv *bulbNewEnv(bulbEnv *upperEnv);

#endif
