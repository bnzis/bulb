/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */
#include "gc.h"

bulbCons bulbObjPoolCons = {NULL, NULL};
bulbObj bulbObjPoolObj = {bulbPrintCons, &bulbObjPoolCons};
bulbObj *bulbObjPool = &bulbObjPoolObj;
bulbObj *bulbObjPoolFront = &bulbObjPoolObj;

bulbObj *bulbNewObj()
{
    bulbObj *obj = (bulbObj*) malloc(sizeof(bulbObj));
    obj->reached = false;
    bulbSetCar(bulbObjPoolFront, obj);
    bulbObj *newFront = (bulbObj*) malloc(sizeof(bulbObj));
    newFront->type = BULB_CONS;
    newFront->data = (bulbCons*) malloc(sizeof(bulbCons));;
    bulbSetCdr(bulbObjPoolFront, newFront); 
    bulbObjPoolFront = bulbGetCdr(bulbObjPoolFront);
    return obj;
}

void bulbGCMarkObj(bulbObj *obj)
{
    if (obj == bulbNil || obj == bulbTrue || obj == bulbFalse
            || obj->type == BULB_PRIMITIVE) return;
    if (obj->reached == true) return;
    obj->reached = true;
    printf("marking: ");
    bulbPrintAst(obj);
    puts("");
    if (obj->type == BULB_PROCEDURE) {
        bulbGCMarkObj(bulbGetProcArgs(obj));
        bulbGCMarkObj(bulbGetProcBody(obj));
        bulbGCMarkEnv(bulbGetProcEnv(obj));
    } else if (obj->type == BULB_CONS) {
        bulbGCMarkObj(bulbGetCar(obj));
        bulbGCMarkObj(bulbGetCdr(obj));
    }
}

void bulbGCMarkEnv(bulbEnv *env)
{
    if (!env) return;
    bulbHashmap *map = env->local;
    for (unsigned i = 0; i < HMAP_ROWS; i++)
        if (map->data[i]) bulbGCMarkObj(map->data[i]);
}

void bulbGCFreeObj(bulbObj *obj)
{
    if (!obj || obj->type == BULB_PRIMITIVE || obj == bulbNil || obj == bulbTrue
             || obj == bulbFalse) return;
    printf("freeing: \n");
    if (obj->type == BULB_CONS) {
        printf("cons\n");
        bulbGCFreeObj(bulbGetCar(obj));
        bulbGCFreeObj(bulbGetCdr(obj));
    } else if (obj->type == BULB_PROCEDURE) {
        printf("proc\n");
        bulbGCFreeObj(bulbGetProcArgs(obj));
        bulbGCFreeObj(bulbGetProcBody(obj));
    } else if (obj->data) {
        printf("other\n");
        free(obj->data);
    }
    free(obj);
}

void bulbGCUnmarkObj(bulbObj *obj)
{
    if (!obj || obj == bulbNil || obj == bulbTrue || obj == bulbFalse
             || obj->type == BULB_PRIMITIVE) return;
    if (obj->reached == false) return;
    obj->reached = false;
    printf("unmarking: ");
    bulbPrintAst(obj);
    puts("");
    if (obj->type == BULB_PROCEDURE) {
        bulbGCUnmarkObj(bulbGetProcArgs(obj));
        bulbGCUnmarkObj(bulbGetProcBody(obj));
        bulbGCMarkEnv(bulbGetProcEnv(obj));
    } else if (obj->type == BULB_CONS) {
        bulbGCUnmarkObj(bulbGetCar(obj));
        bulbGCUnmarkObj(bulbGetCdr(obj));
    }
}

void bulbGCSweep()
{
    bulbObj *front = bulbObjPool;
    while (front) { 
        if (bulbGetCar(front)) {
            if (bulbGetCar(front)->reached) 
                bulbGCUnmarkObj(bulbGetCar(front));
            else {
                bulbGCFreeObj(bulbGetCar(front));
                bulbMakeCons(front)->car = NULL;
            }
        }
        front = bulbGetCdr(front);
    }
}

bulbObj *bulbNewConsObj(bulbObj *car, bulbObj *cdr)
{
    bulbObj *cons = bulbNewObj();
    cons->type = BULB_CONS;
    cons->data = (bulbCons*) malloc(sizeof(bulbCons));
    bulbSetCar(cons, car);
    bulbSetCdr(cons, cdr);
    return cons;
}

bulbObj *bulbNewStringObj(char *text, unsigned len)
{
    bulbObj *string = bulbNewObj();
    string->type = BULB_STRING;
    string->data = (bulbString*) malloc(sizeof(bulbString*));
    ((bulbString*) string->data)->data = text;
    ((bulbString*) string->data)->len = len;
    return string;
}

bulbObj *bulbNewProcObj(bulbObj *args, bulbObj *body, bulbEnv *env)
{
    bulbObj *proc = bulbNewObj();
    proc->type = BULB_PROCEDURE;
    proc->data = (bulbProc*) malloc(sizeof(bulbProc));
    ((bulbProc*) proc->data)->args = args;
    ((bulbProc*) proc->data)->body = body;
    ((bulbProc*) proc->data)->env = env;
    return proc;
}

bulbEnv *bulbNewEnv(bulbEnv *upperEnv)
{
    bulbEnv *env = (bulbEnv*) malloc(sizeof(bulbEnv));
    env->local = (bulbHashmap*) malloc(sizeof(bulbHashmap));
    env->local->data = (bulbObj**) calloc(HMAP_ROWS, sizeof(bulbObj*));
    env->upperEnv = upperEnv;
    return env;
}
