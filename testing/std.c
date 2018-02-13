/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "std.h"

bulbObj *bulbSTDSum(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = BULB_INT;
    result->data = (bulbInt*) malloc(sizeof(bulbInt));
    *((bulbInt*) result->data) = 0;
    while (args != bulbNil) {
        if (bulbGetCar(args)->type == BULB_FLOAT) {
            if (result->type == BULB_INT) {
                bulbInt tmp = *((bulbInt*) result->data);
                free(result->data);
                result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
                result->type = BULB_FLOAT;
                *((bulbFloat*) result->data) = tmp;
            }
            *((bulbFloat*) result->data) += *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT)
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
        args = bulbGetCdr(args);
    }
    return result;
}

