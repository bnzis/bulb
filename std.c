/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "std.h"

bulbPrimitive bulbSTDPrimitives[29] = { bulbSTDSum, bulbSTDMul, bulbSTDSub, 
                                bulbSTDDiv, bulbSTDSqrt, bulbSTDOperatorEqual, 
                                bulbSTDOperatorNotEqual, bulbSTDOperatorBigger, 
                                bulbSTDOperatorSmaller, bulbSTDOperatorBiggerEqual, 
                                bulbSTDOperatorSmallerEqual, bulbSTDAnd, 
                                bulbSTDOr, bulbSTDNot, bulbSTDPrint,
                                bulbSTDExit, bulbSTDCons, bulbSTDCar, bulbSTDCdr, 
                                bulbSTDSetCar, bulbSTDSetCdr, bulbSTDList, 
                                bulbSTDIsNull, bulbSTDIsList, bulbSTDIsAtom,
                                bulbSTDIsString, bulbSTDIsNumber, NULL
};

char *bulbSTDNames[29] = { "+", "*", "-", "/", "sqrt", "=", "!=", ">", "<", ">=", "<=",
                           "and", "or", "not", "print", "exit", "cons", "car", "cdr", 
                           "set-car!", "set-cdr!", "list", "null?", "list?", "atom?", 
                           "string?", "number?", NULL
};

bulbModule bulbSTDModule = {bulbSTDPrimitives, bulbSTDNames};

bulbModule *bulbSTD = &bulbSTDModule;

bulbObj *bulbSTDSum(bulbObj *args, bulbEnv *env)
{
    if (args == bulbNil) bulb_err_invalid_len_name("+", 1, 0);
    bulbObj *result = bulbNewObj();
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
    if (args == bulbNil) bulb_err_invalid_len_name("-", 1, 0);
    bulbObj *result = bulbNewObj();
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
    if (args == bulbNil) bulb_err_invalid_len_name("*", 1, 0);
    bulbObj *result = bulbNewObj();
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
    if (args == bulbNil) bulb_err_invalid_len_name("/", 1, 0);
    bulbObj *result = bulbNewObj();
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

bulbObj *bulbSTDSqrt(bulbObj *args, bulbEnv *env) 
{
    if (args == bulbNil) bulb_err_expected_number("sqrt", args);
    bulbObj *result = bulbNewObj();
    result->type = bulbGetCar(args)->type;
    if (result->type == BULB_INT) {
        result->data = (bulbInt*) malloc(sizeof(bulbInt));
        memcpy(result->data, bulbGetCar(args)->data, sizeof(bulbObj));
        *((bulbInt*) result->data) = sqrt(*((bulbInt*) result->data)); 
    } else if (result->type == BULB_FLOAT) {
        result->data = (bulbFloat*) malloc(sizeof(bulbFloat));
        memcpy(result->data, bulbGetCar(args)->data, sizeof(bulbObj));
        *((bulbFloat*) result->data) = sqrt(*((bulbFloat*) result->data)); 
    } else bulb_err_expected_number("sqrt", bulbGetCar(args));
    return result;
}

bulbObj *bulbSTDOperatorEqual(bulbObj *args, bulbEnv *env)
{
    if (args == bulbNil) bulb_err_invalid_len_name("=", 1, 0);
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
    if (args == bulbNil) bulb_err_invalid_len_name("!=", 1, 0);
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
    if (args == bulbNil) bulb_err_invalid_len_name(">", 1, 0);
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
    if (args == bulbNil) bulb_err_invalid_len_name("<", 1, 0);
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
    if (args == bulbNil) bulb_err_invalid_len_name(">=", 1, 0);
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
    if (args == bulbNil) bulb_err_invalid_len_name("<=", 1, 0);
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

bulbObj *bulbSTDAnd(bulbObj *args, bulbEnv *env) 
{
    if (args == bulbNil) bulb_err_invalid_len_name("and", 1, 0);
    bulbObj *result = bulbTrue;
    while (args != bulbNil && result == bulbTrue) {
        if (bulbGetCar(args)->type == BULB_BOOL) 
            result = bulbGetCar(args);
        else bulb_err_expected_bool("and", bulbGetCar(args));
        args = bulbGetCdr(args);
    } 
    return result;
}

bulbObj *bulbSTDOr(bulbObj *args, bulbEnv *env) 
{
    if (args == bulbNil) bulb_err_invalid_len_name("or", 1, 0);
    bulbObj *result = bulbFalse;
    while (args != bulbNil && result == bulbFalse) {
        if (bulbGetCar(args)->type == BULB_BOOL) 
            result = bulbGetCar(args);
        else bulb_err_expected_bool("or", bulbGetCar(args));
        args = bulbGetCdr(args);
    } 
    return result;
}

bulbObj *bulbSTDNot(bulbObj *args, bulbEnv *env) 
{
    if (args == bulbNil) bulb_err_invalid_len_name("not", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("not", 1, len);
    if (bulbGetCar(args) == bulbTrue) return bulbFalse;
    else if (bulbGetCar(args) == bulbFalse) return bulbTrue;
    else bulb_err_expected_bool("not", bulbGetCar(args));
    return bulbNil;
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
    if (len != 0) bulb_err_invalid_len_name("exit", 0, len);
    exit(0);
}

bulbObj *bulbSTDCons(bulbObj *args, bulbEnv *env)
{   
    unsigned len = bulbListLen(args);
    if (len != 2) bulb_err_invalid_len_name("cons", 2, len);
    bulbObj *cons = bulbNewConsObj(bulbGetCar(args), bulbGetCadr(args));
    return cons;
}

bulbObj *bulbSTDCar(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("car", 1, len);
    if (bulbGetCar(args)->type != BULB_CONS) 
        bulb_err_expected_cons("car", bulbGetCar(args));
    return bulbGetCaar(args);
}

bulbObj *bulbSTDCdr(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("cdr", 1, len);
    if (bulbGetCar(args)->type != BULB_CONS) 
        bulb_err_expected_cons("cdr", bulbGetCar(args));
    return bulbGetCdar(args);
}

bulbObj *bulbSTDSetCar(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 2) bulb_err_invalid_len_name("set-car!", 2, len);
    if (bulbGetCar(args)->type != BULB_CONS) 
        bulb_err_expected_cons("set-car!", bulbGetCar(args));
    bulbSetCar(bulbGetCar(args), bulbGetCadr(args));
    return bulbGetCar(args);
}


bulbObj *bulbSTDSetCdr(bulbObj *args, bulbEnv *env)
{
    unsigned len = bulbListLen(args);
    if (len != 2) bulb_err_invalid_len_name("set-cdr!", 2, len);
    if (bulbGetCar(args)->type != BULB_CONS) 
        bulb_err_expected_cons("set-cdr!", bulbGetCar(args));
    bulbSetCdr(bulbGetCar(args), bulbGetCadr(args));
    return bulbGetCar(args);
}

bulbObj *bulbSTDList(bulbObj *args, bulbEnv *env)
{
    return args;
}

bulbObj *bulbSTDIsNull(bulbObj *args, bulbEnv *env)
{   
    if (args == bulbNil) bulb_err_invalid_len_name("null?", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("null?", 1, len);
    return (bulbGetCar(args) == bulbNil)? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDIsList(bulbObj *args, bulbEnv *env)
{   
    if (args == bulbNil) bulb_err_invalid_len_name("list?", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("list?", 1, len);
    return (bulbIsAList(bulbGetCar(args)))? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDIsAtom(bulbObj *args, bulbEnv *env)
{   
    if (args == bulbNil) bulb_err_invalid_len_name("atom?", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("atom?", 1, len);
    return (bulbGetCar(args)->type != BULB_CONS)? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDIsString(bulbObj *args, bulbEnv *env)
{   
    if (args == bulbNil) bulb_err_invalid_len_name("string?", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("string?", 1, len);
    return (bulbGetCar(args)->type == BULB_STRING)? bulbTrue : bulbFalse;
}

bulbObj *bulbSTDIsNumber(bulbObj *args, bulbEnv *env)
{   
    if (args == bulbNil) bulb_err_invalid_len_name("number?", 1, 0);
    unsigned len = bulbListLen(args);
    if (len != 1) bulb_err_invalid_len_name("number?", 1, len);
    return (bulbGetCar(args)->type == BULB_INT ||
            bulbGetCar(args)->type == BULB_FLOAT)? bulbTrue : bulbFalse;
}

void bulb_err_expected_number(char *name, bulbObj *o)
{
    printf("Exception: %s: expected a number, but got ", name);
    bulbPrintAst(o);
    printf(".\n");
    exit(1);
}

void bulb_err_invalid_len_name(char *name, unsigned expected, unsigned given)
{
    printf("Exception: %s: incorrect number of arguments (expected %d, given %d).\n", 
            name, expected, given);
    exit(1);
}

void bulb_err_expected_bool(char *name, bulbObj *o)
{
    printf("Exception: %s: expected a boolean, but got ", name);
    bulbPrintAst(o);
    printf(".\n");
    exit(1);
}

void bulb_err_expected_cons(char *name, bulbObj *o)
{
    printf("Exception: %s: expected a pair, but got ", name);
    bulbPrintAst(o);
    printf(".\n");
    exit(1);
}
