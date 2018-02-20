/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "std.h"

bulbPrimitive bulbSTDPrimitives[16] = { bulbSTDSum, bulbSTDMul, bulbSTDSub, 
                                bulbSTDDiv, bulbSTDOperatorEqual, 
                                bulbSTDOperatorNotEqual, bulbSTDOperatorBigger, 
                                bulbSTDOperatorSmaller, bulbSTDOperatorBiggerEqual, 
                                bulbSTDOperatorSmallerEqual, bulbSTDDisplay,
                                bulbSTDExit, bulbSTDCons, bulbSTDCar, bulbSTDCdr, NULL
};

char *bulbSTDNames[16] = { "+", "*", "-", "/", "=", "!=", ">", "<", ">=", "<=",
                           "display", "exit", "cons", "car", "cdr", NULL
};

bulbModule bulbSTDModule = {bulbSTDPrimitives, bulbSTDNames};

bulbModule *bulbSTD = &bulbSTDModule;

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
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT) 
                *((bulbFloat*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
        }
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDSub(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = BULB_INT;
    result->data = (bulbInt*) malloc(sizeof(bulbInt));
    memcpy(result->data, bulbGetCar(args)->data, sizeof(bulbObj));
    args = bulbGetCdr(args);
    while (args != bulbNil) {
        if (bulbGetCar(args)->type == BULB_FLOAT) {
            if (result->type == BULB_INT) {
                bulbInt tmp = *((bulbInt*) result->data);
                free(result->data);
                result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
                result->type = BULB_FLOAT;
                *((bulbFloat*) result->data) = tmp;
            }
            *((bulbFloat*) result->data) -= *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) -= *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) -= *((bulbInt*) bulbGetCar(args)->data);
        }
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDMul(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = BULB_INT;
    result->data = (bulbInt*) malloc(sizeof(bulbInt));
    *((bulbInt*) result->data) = 1;
    while (args != bulbNil) {
        if (bulbGetCar(args)->type == BULB_FLOAT) {
            if (result->type == BULB_INT) {
                bulbInt tmp = *((bulbInt*) result->data);
                free(result->data);
                result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
                result->type = BULB_FLOAT;
                *((bulbFloat*) result->data) = tmp;
            }
            *((bulbFloat*) result->data) *= *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) *= *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) *= *((bulbInt*) bulbGetCar(args)->data);
        }
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDDiv(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = BULB_INT;
    result->data = (bulbInt*) malloc(sizeof(bulbInt));
    memcpy(result, bulbGetCar(args), sizeof(bulbObj));
    args = bulbGetCdr(args);
    while (args != bulbNil) {
        if (bulbGetCar(args)->type == BULB_FLOAT) {
            if (result->type == BULB_INT) {
                bulbInt tmp = *((bulbInt*) result->data);
                free(result->data);
                result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
                result->type = BULB_FLOAT;
                *((bulbFloat*) result->data) = tmp;
            }
            *((bulbFloat*) result->data) /= *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) /= *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) /= *((bulbInt*) bulbGetCar(args)->data);
        }
        args = bulbGetCdr(args);
    }
    return result;
}


bulbObj *bulbSTDOperatorEqual(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) == *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) == *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) == *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) == *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDOperatorNotEqual(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) != *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) != *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) != *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) != *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDOperatorBigger(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) > *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) > *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) > *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) > *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDOperatorSmaller(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) < *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) < *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) < *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) < *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDOperatorBiggerEqual(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) >= *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) >= *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) >= *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) >= *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDOperatorSmallerEqual(bulbObj *args, bulbEnv *env)
{
    bool res = true;
    while (bulbGetCdr(args) != bulbNil && res) {
        if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbInt*) bulbGetCar(args)->data) <= *((bulbInt*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbFloat*) bulbGetCar(args)->data) <= *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_INT && bulbGetCadr(args)->type == BULB_FLOAT)
            res = *((bulbInt*) bulbGetCar(args)->data) <= *((bulbFloat*) bulbGetCadr(args)->data);
        else if (bulbGetCar(args)->type == BULB_FLOAT && bulbGetCadr(args)->type == BULB_INT)
            res = *((bulbFloat*) bulbGetCar(args)->data) <= *((bulbInt*) bulbGetCadr(args)->data);
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDDisplay(bulbObj *args, bulbEnv *env)
{
    while (args != bulbNil) {
        bulbPrintAstDisplay(bulbGetCar(args), false);
        args = bulbGetCdr(args);
    }
    return bulbNil;
}

bulbObj *bulbSTDExit(bulbObj *args, bulbEnv *env)
{
    exit(*(bulbInt*) bulbGetCar(args)->data);
}

bulbObj *bulbSTDCons(bulbObj *args, bulbEnv *env)
{
    bulbObj *cons = bulbNewConsObj(bulbGetCar(args), bulbGetCadr(args));
    return cons;
}

bulbObj *bulbSTDCar(bulbObj *args, bulbEnv *env)
{
    return bulbGetCaar(args);
}

bulbObj *bulbSTDCdr(bulbObj *args, bulbEnv *env)
{
    return bulbGetCdar(args);
}
