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
    result->data.integer = 0;
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->data.integer;
                result->data.floating = tmp;
                result->type = FLOAT;
            }
            result->data.floating += car(args)->data.floating;
        } else if (args->data.cons.car->type == INT) 
            if (result->type == FLOAT)
                result->data.floating += car(args)->data.integer;
            else
                result->data.integer += car(args)->data.integer;
        args = cdr(args);
    }
    return result;
}

obj_t *multiply(obj_t *args, env_t *env)
{
    obj_t *result = malloc(sizeof(obj_t));
    result->type = INT;
    result->data.integer = 1;
    while (car(args) != NULL) {
        if (car(args)->type == FLOAT) {
            if (result->type == INT) {
                int tmp = result->data.integer;
                result->data.floating = tmp;
                result->type = FLOAT;
            }
            result->data.floating *= car(args)->data.floating;
        } else if (args->data.cons.car->type == INT)
            if (result->type == FLOAT)
                result->data.floating *= car(args)->data.integer;
            else
                result->data.integer *= car(args)->data.integer;
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
                int tmp = result->data.integer;
                result->data.floating = tmp;
                result->type = FLOAT;
            }
            result->data.floating -= car(args)->data.floating;
        } else if (args->data.cons.car->type == INT) 
            if (result->type == FLOAT)
                result->data.floating -= car(args)->data.integer;
            else
                result->data.integer -= car(args)->data.integer;
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
                int tmp = result->data.integer;
                result->data.floating = tmp;
                result->type = FLOAT;
            }
            result->data.floating /= car(args)->data.floating;
        } else if (args->data.cons.car->type == INT)
            if (result->type == FLOAT)
                result->data.floating /= car(args)->data.integer;
            else
                result->data.integer /= car(args)->data.integer;
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
            res = car(args)->data.integer == cadr(args)->data.integer;
        if (car(args)->type == FLOAT && cadr(args)->type == FLOAT)
            res = car(args)->data.floating == cadr(args)->data.floating;
        if (car(args)->type == INT && cadr(args)->type == FLOAT)
            res = car(args)->data.integer == cadr(args)->data.floating;
        if (car(args)->type == FLOAT && cadr(args)->type == INT)
            res = car(args)->data.floating == cadr(args)->data.integer;
        args = cdr(args);
    }
    result->data.boolean = res;
    return result;
}
