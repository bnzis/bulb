/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_STD_H
#define BULB_STD_H
#include <string.h>
#include "core.h"

/* STD
   ---
   This module contains all basic functions available on bulb. */   
extern bulbModule *bulbSTD;

/* Mathematical Operations with numbers 
   ------------------------------------ 
   +, *, -, / */
bulbObj *bulbSTDSum(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDMul(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDSub(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDDiv(bulbObj *args, bulbEnv *env);

/* Relational Operations with numbers
   ----------------------------------- 
   =, !=, >, <, >=, <= */
bulbObj *bulbSTDOperatorEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorNotEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorBigger(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorSmaller(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorBiggerEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorSmallerEqual(bulbObj *args, bulbEnv *env);

/* Operations with Booleans 
   ------------------------ 
   and, or, not */
bulbObj *bulbSTDAnd(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOr(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDNot(bulbObj *args, bulbEnv *env);

/* Extra functions
   --------------- */
bulbObj *bulbSTDPrint(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDExit(bulbObj *args, bulbEnv *env);

/* Pairs and Lists 
   --------------- */
bulbObj *bulbSTDCons(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDCar(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDCdr(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDSetCar(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDSetCdr(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDList(bulbObj *args, bulbEnv *env);

/* Exceptions
   ---------- */
void bulb_err_expected_number(char *name, bulbObj *o);

void bulb_err_invalid_len_name(char *name, unsigned expected, unsigned given);

void bulb_err_expected_bool(char *name, bulbObj *o);

void bulb_err_expected_cons(char *name, bulbObj *o);

#endif /* BULB_STD_H */
