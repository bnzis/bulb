/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016 Yann Collet (xxhash)
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "core.h"

obj_t *sum(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    result->type = INT;
    result->integer = 0;
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->integer;
                result->floating = tmp;
                result->type = FLOAT;
            }
            result->floating += car(args)->floating;
        } else if (args->cons.car->type == INT) 
            if (result->type == FLOAT)
                result->floating += car(args)->integer;
            else
                result->integer += car(args)->integer;
        args = cdr(args);
    }
    return result;
}

obj_t *multiply(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    result->type = INT;
    result->integer = 1;
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->integer;
                result->floating = tmp;
                result->type = FLOAT;
            }
            result->floating *= car(args)->floating;
        } else if (args->cons.car->type == INT)
            if (result->type == FLOAT)
                result->floating *= car(args)->integer;
            else
                result->integer *= car(args)->integer;
        args = cdr(args);
    }
    return result;
}

obj_t *substract(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    memcpy(result, car(args), sizeof(obj_t));
    args = cdr(args);
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->integer;
                result->floating = tmp;
                result->type = FLOAT;
            }
            result->floating -= car(args)->floating;
        } else if (args->cons.car->type == INT) 
            if (result->type == FLOAT)
                result->floating -= car(args)->integer;
            else
                result->integer -= car(args)->integer;
        args = cdr(args);
    }
    return result;
}

obj_t *divide(obj_t *args, env_t *env)
{
    obj_t *result = car(args);
    args = cdr(args);
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->integer;
                result->floating = tmp;
                result->type = FLOAT;
            }
            result->floating /= car(args)->floating;
        } else if (args->cons.car->type == INT)
            if (result->type == FLOAT)
                result->floating /= car(args)->integer;
            else
                result->integer /= car(args)->integer;
        args = cdr(args);
    }
    return result;
}

obj_t *operator_equal(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer == cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating == cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer == cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating == cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}

obj_t *operator_not_equal(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer != cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating != cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer != cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating != cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}

obj_t *operator_bigger(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer > cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating > cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer > cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating > cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}

obj_t *operator_smaller(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer < cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating < cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer < cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating < cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}

obj_t *operator_bigger_equal(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer >= cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating >= cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer >= cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating >= cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}

obj_t *operator_smaller_equal(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    bool res = true;
    result->type = BOOL;
    while (cdr(args) != NULL && res) {
        if (car(args)->type == INT && cadr(args)->type == INT)
            res = car(args)->integer <= cadr(args)->integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->floating <= cadr(args)->floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->integer <= cadr(args)->floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->floating <= cadr(args)->integer;
        args = cdr(args);
    }
    result->boolean = res;
    return result;
}
