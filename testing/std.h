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

/* Comparative Operations with numbers
   ----------------------------------- 
   =, !=, >, <, >=, <= */
bulbObj *bulbSTDOperatorEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorNotEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorBigger(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorSmaller(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorBiggerEqual(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDOperatorSmallerEqual(bulbObj *args, bulbEnv *env);

/* Other functions
   --------------- */
bulbObj *bulbSTDDisplay(bulbObj *args, bulbEnv *env);

bulbObj *bulbSTDExit(bulbObj *args, bulbEnv *env);

#endif /* BULB_STD_H */
