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
                                bulbSTDOperatorSmallerEqual, bulbSTDPrint,
                                bulbSTDExit, bulbSTDCons, bulbSTDCar, bulbSTDCdr, NULL
};

char *bulbSTDNames[16] = { "+", "*", "-", "/", "=", "!=", ">", "<", ">=", "<=",
                           "print", "exit", "cons", "car", "cdr", NULL
};

bulbModule bulbSTDModule = {bulbSTDPrimitives, bulbSTDNames};

bulbModule *bulbSTD = &bulbSTDModule;

bulbObj *bulbSTDSum(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = bulbGetCar(args)->type;
    if (result->type == BULB_INT)
        result->data = (bulbInt*) malloc(sizeof(bulbInt));
    else if (result->type == BULB_FLOAT)
        result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
    else bulb_err_expected_number("+", bulbGetCar(args));
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
            *((bulbFloat*) result->data) += *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT) 
                *((bulbFloat*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) += *((bulbInt*) bulbGetCar(args)->data);
        } else bulb_err_expected_number("+", bulbGetCar(args));
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDSub(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = bulbGetCar(args)->type;
    if (result->type == BULB_INT)
        result->data = (bulbInt*) malloc(sizeof(bulbInt));
    else if (result->type == BULB_FLOAT)
        result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
    else bulb_err_expected_number("-", bulbGetCar(args));
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
        } else bulb_err_expected_number("-", bulbGetCar(args));
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDMul(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = bulbGetCar(args)->type;
    if (result->type == BULB_INT)
        result->data = (bulbInt*) malloc(sizeof(bulbInt));
    else if (result->type == BULB_FLOAT)
        result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
    else bulb_err_expected_number("*", bulbGetCar(args));
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
            *((bulbFloat*) result->data) *= *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) *= *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) *= *((bulbInt*) bulbGetCar(args)->data);
        } else bulb_err_expected_number("*", bulbGetCar(args));
        args = bulbGetCdr(args);
    }
    return result;
}

bulbObj *bulbSTDDiv(bulbObj *args, bulbEnv *env)
{
    bulbObj *result = (bulbObj*) malloc(sizeof(bulbObj));
    result->type = bulbGetCar(args)->type;
    if (result->type == BULB_INT)
        result->data = (bulbInt*) malloc(sizeof(bulbInt));
    else if (result->type == BULB_FLOAT)
        result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
    else bulb_err_expected_number("/", bulbGetCar(args));
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
            *((bulbFloat*) result->data) /= *((bulbFloat*) bulbGetCar(args)->data);
        } else if (bulbGetCar(args)->type == BULB_INT) {
            if (result->type == BULB_FLOAT)
                *((bulbFloat*) result->data) /= *((bulbInt*) bulbGetCar(args)->data);
            else
                *((bulbInt*) result->data) /= *((bulbInt*) bulbGetCar(args)->data);
        } else bulb_err_expected_number("/", bulbGetCar(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number("=", bulbGetCar(args));
        else bulb_err_expected_number("=", bulbGetCadr(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number("!=", bulbGetCar(args));
        else bulb_err_expected_number("!=", bulbGetCadr(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number(">", bulbGetCar(args));
        else bulb_err_expected_number(">", bulbGetCadr(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number("<", bulbGetCar(args));
        else bulb_err_expected_number("<", bulbGetCadr(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number(">=", bulbGetCar(args));
        else bulb_err_expected_number(">=", bulbGetCadr(args));
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
        else if (bulbGetCar(args)->type != BULB_INT && bulbGetCar(args)->type != BULB_FLOAT)
            bulb_err_expected_number("<=", bulbGetCar(args));
        else bulb_err_expected_number("<=", bulbGetCadr(args));
        args = bulbGetCdr(args);
    }
    return (res) ? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDPrint(bulbObj *args, bulbEnv *env)
{
    while (args != bulbNil) {
        bulbPrintAstDisplay(bulbGetCar(args), false);
        args = bulbGetCdr(args);
    }
    return bulbNil;
}

bulbObj *bulbSTDExit(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 0) bulb_err_invalid_len(0, len);
    exit(0);
}

bulbObj *bulbSTDCons(bulbObj *args, bulbEnv *env)
{   
    unsigned len = bulbListLen(args);
    if (len != 2) bulb_err_invalid_len(2, len);
    bulbObj *cons = bulbNewConsObj(bulbGetCar(args), bulbGetCadr(args));
    return cons;
}

bulbObj *bulbSTDCar(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len(1, len);
    return bulbGetCaar(args);
}

bulbObj *bulbSTDCdr(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len(1, len);
    return bulbGetCdar(args);
}

void bulb_err_expected_number(char *name, bulbObj *o)
{
    printf("Exception: %s: expected a number, but got ", name);
    bulbPrintAst(o);
    printf(".\n");
    exit(1);
}
